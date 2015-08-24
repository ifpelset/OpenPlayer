/**
  * 获取mp3中的IDV3标签的专辑图片
  * 因为Qt自身的该功能有问题，所以网上找了纯c版本并一移植成Qt版本
  * 无任何优化修改，只做移植功能
  *
  * @By IFPELSET(Pt)
  * @Date 2015-8-24
  */
#include <QDebug>
#include <QFile>

typedef unsigned char byte;

//id3v2 标签头结构体
typedef struct
{
    char  identi[3];//必须为ID3，否则认为标签不存在
    byte  major;    //版本号
    byte  revsion;  //副版本号
    byte  flags;    //标志位
    byte  size[4]; //标签大小，不包括标签头的10个字节
}ID3V2Header;

//标签帧头结构体
typedef struct
{
    char FrameId[4];//标签帧标识符，用来表明这个标签帧的内容
    byte size[4];    //此标签帧的大小，不包括此标签头的10个字节
    byte flags[2];  //标志位
}ID3V2Frameheader;

QFile *getAudioLabelHeaderLen(const QString &audioName, int &len)
{
    QFile *pFile = new QFile(audioName);
    if (!pFile->open(QIODevice::ReadOnly)) {
        qDebug() << "Can not open file";
        return NULL;
    }

    pFile->seek(0);

    //读取标签头
    ID3V2Header mp3ID3V2;
    memset(&mp3ID3V2, 0, 10);
    pFile->read((char *)&mp3ID3V2, 10);

    if (0 != strncmp(mp3ID3V2.identi, "ID3", 3)) {
        qDebug() << "No have ID3V2 label";
        pFile->close();
        return NULL;
    }

    len = (mp3ID3V2.size[0]&0x7f)*0x200000
            +(mp3ID3V2.size[1]&0x7f)*0x4000
            +(mp3ID3V2.size[2]&0x7f)*0x80
            +(mp3ID3V2.size[3]&0x7f);

    return pFile;
}

bool getPicInfo(QFile *pFile, int &dwFrame, int len, int &tempi)
{
    ID3V2Frameheader pFrameBuf;
    memset(&pFrameBuf, 0, 10);
    pFile->read((char *)&pFrameBuf, 10);

    int i = 0;
    //找到图片标签的所在位置
    while((strncmp(pFrameBuf.FrameId,"APIC",4) != 0))
    {
        //判断是否有图片
        if(i>len)
        {
            qDebug() << "没有找到标识图像帧的标签";
            return false;
        }
        dwFrame= pFrameBuf.size[0]*0x1000000
            +pFrameBuf.size[1]*0x10000
            +pFrameBuf.size[2]*0x100
            +pFrameBuf.size[3];
        pFile->seek(pFile->pos() + dwFrame);
        memset(&pFrameBuf, 0, 10);
        pFile->read((char *)&pFrameBuf, 10);
        i++;
    }

    //计算出图片标签的大小
    dwFrame= pFrameBuf.size[0]*0x1000000
        +pFrameBuf.size[1]*0x10000
        +pFrameBuf.size[2]*0x100
        +pFrameBuf.size[3];

    char image_tag[7]={"0"};
    char pic_type[5]={"0"};
    pFile->read(image_tag, 6);
    //fread(image_tag,6,1,fp);
    //判断图片格式
    i=0;
    while(true)
    {
        if(i>dwFrame)
        {
            qDebug() << "没有找到标识图像类型的标签";
            pFile->close();
            return false;
        }
        if(0==(strcmp(image_tag,"image/")))
        {
            tempi+=6;
            pFile->read(pic_type, 4);
            //mp3里面大多图片都是jpeg，也是以jpeg作为标志的
            //也有以jpe，jpg，peg作为标志的
            //不过也有png等格式的。
            if(0==strncmp(pic_type,"jpeg",4))
            {
                tempi+=4;
                break;
            }
            else if(0==strncmp(pic_type,"jpg",3))
            {
                tempi+=3;
                pFile->seek(pFile->pos() - 1);
                break;
            }
            else if(0==strncmp(pic_type,"peg",3))
            {
                tempi+=3;
                pFile->seek(pFile->pos() - 1);
                break;
            }
            else
            {
                qDebug() << "图片格式不是jpeg";
                pFile->close();
                return false;
            }
        }
        else
        {
            i++;
            pFile->seek(pFile->pos() - 5);
            pFile->read(image_tag, 6);
            tempi=tempi+1;
            continue;
        }
    }
    return true;
}

char *getPicRGB(QFile *pFile, int dwFrame, int tempi)
{
    byte  *pPicData;

    //这两个tag的是表明图片数据的开始
    //jpeg图片开始的标志是0xFFD8
    byte jpeg_header_tag1;
    byte jpeg_header_tag2;
    pFile->seek(pFile->pos());
    pFile->read((char *)&jpeg_header_tag1, 1);
    pFile->seek(pFile->pos());
    pFile->read((char *)&jpeg_header_tag2, 1);

    //计算出图片数据开始的地方
    int i=0;
    while(true)
    {
        if(i>dwFrame)
        {
            qDebug() << "没有找到图像数据";
            pFile->close();
        }
        i++;
        if((255==jpeg_header_tag1) && (216==jpeg_header_tag2))
        {
            pPicData = new byte[dwFrame-tempi];
            memset(pPicData, 0, dwFrame-tempi);
            //设定文件流的指针位置，并把图片的数据读入pPicData
            pFile->seek(pFile->pos() - 2);
            pFile->read((char *)pPicData, dwFrame-tempi);
            pFile->close();

            pFile->setFileName("D:/temp.jpeg");
            if (!pFile->open(QIODevice::WriteOnly)) {
                qDebug() << "Can not open file";
                return NULL;
            }
            pFile->write((const char *)pPicData, dwFrame-tempi);
            break;
        }
        else
        {
            pFile->seek(pFile->pos() - 1);
            pFile->read((char *)&jpeg_header_tag1, 1);
            pFile->seek(pFile->pos());
            pFile->read((char *)&jpeg_header_tag2, 1);
            tempi++;
            continue;
        }
    }
    pFile->close();
    return (char *)pPicData;
}

char * getAlbumPicture(const QString &audioName)
{
    int len, dwFrame = 0, tempi = 0;

    QFile *pFile = getAudioLabelHeaderLen(audioName, len);

    if (pFile == NULL)
        return NULL;

    bool bRet = getPicInfo(pFile, dwFrame, len, tempi);

    if (!bRet)
        return NULL;

    return getPicRGB(pFile, dwFrame, tempi);
}
