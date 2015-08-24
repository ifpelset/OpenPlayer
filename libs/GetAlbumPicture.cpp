#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <Windows.h>

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

const char *MusicName="10086.mp3";
long width = 0;
long height = 0;

FILE *OpenMusic(int &Len)
{
    FILE *fp=NULL;
    fp=fopen(MusicName,"rb");
    if(!fp)
    {
        printf("无法打开文件\n");
        fp=NULL;
        return fp;
    }
    //把打开的音乐文件流的内部指针重置到文件开头
    fseek(fp,0,SEEK_SET);

    //读取标签头
    ID3V2Header mp3ID3V2;
    ZeroMemory(&mp3ID3V2,10);
    fread(&mp3ID3V2,10,1,fp);
    //判断有没有id3v2的标签头
    if(0!=strncmp(mp3ID3V2.identi,"ID3",3))
    {
        printf("没有ID2V2标签\n");
        fclose(fp);
        fp=NULL;
        return fp;
    }

    //计算处整个标签的大小
    Len = (mp3ID3V2.size[0]&0x7f)*0x200000
        +(mp3ID3V2.size[1]&0x7f)*0x4000
        +(mp3ID3V2.size[2]&0x7f)*0x80
        +(mp3ID3V2.size[3]&0x7f);

    return fp;
}

BOOL GetPicInfo(FILE *fp, int &dwFrame, int Len, int &tempi)
{
    ID3V2Frameheader pFrameBuf;
    ZeroMemory(&pFrameBuf,10);
    fread(&pFrameBuf,10,1,fp);
    int i=0;
    //找到图片标签的所在位置
    while((strncmp(pFrameBuf.FrameId,"APIC",4) != 0))
    {
        //判断是否有图片
        if(i>Len)
        {
            printf("没有找到标识图像帧的标签\n");
            return FALSE;
        }
        dwFrame= pFrameBuf.size[0]*0x1000000
            +pFrameBuf.size[1]*0x10000
            +pFrameBuf.size[2]*0x100
            +pFrameBuf.size[3];
        fseek(fp,dwFrame,SEEK_CUR);
        ZeroMemory(&pFrameBuf,10);
        fread(&pFrameBuf,10,1,fp);
        i++;
    }

    //计算出图片标签的大小
    dwFrame= pFrameBuf.size[0]*0x1000000
        +pFrameBuf.size[1]*0x10000
        +pFrameBuf.size[2]*0x100
        +pFrameBuf.size[3];

    char image_tag[7]={"0"};
    char pic_type[5]={"0"};
    fread(image_tag,6,1,fp);
    //判断图片格式
    i=0;
    while(true)
    {
        if(i>dwFrame)
        {
            printf("没有找到标识图像类型的标签\n");
            fclose(fp);
            return FALSE;
        }
        if(0==(strcmp(image_tag,"image/")))
        {
            tempi+=6;
            fread(pic_type,4,1,fp);
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
                fseek(fp,-1,SEEK_CUR);
                break;
            }
            else if(0==strncmp(pic_type,"peg",3))
            {
                tempi+=3;
                fseek(fp,-1,SEEK_CUR);
                break;
            }
            else
            {
                printf("图片格式不是jpeg\n");
                fclose(fp);
                return FALSE;
            }
        }
        else
        {
            i++;
            fseek(fp,-5,SEEK_CUR);
            fread(image_tag,6,1,fp);
            tempi=tempi+1;
            continue;
        }
    }
    return TRUE;
}

void GetPicRGB(FILE *fp, int dwFrame, int tempi)
{
    TCHAR szTempFileName[MAX_PATH];
    BYTE  *pPicData;
    unsigned char * bmpDataBuffer=NULL;

    //这两个tag的是表明图片数据的开始
    //jpeg图片开始的标志是0xFFD8
    BYTE jpeg_header_tag1;
    BYTE jpeg_header_tag2;
    fseek(fp,0,SEEK_CUR);
    fread(&jpeg_header_tag1,1,1,fp);
    fseek(fp,0,SEEK_CUR);
    fread(&jpeg_header_tag2,1,1,fp);

    //计算出图片数据开始的地方
    int i=0;
    while(true)
    {
        if(i>dwFrame)
        {
            printf("没有找到图像数据\n");
            fclose(fp);
            bmpDataBuffer=NULL;
        }
        i++;
        if((255==jpeg_header_tag1) && (216==jpeg_header_tag2))
        {
            pPicData = new BYTE[dwFrame-tempi];
            ZeroMemory(pPicData,dwFrame-tempi);
            //设定文件流的指针位置，并把图片的数据读入pPicData
            fseek(fp,-2,SEEK_CUR);
            fread(pPicData,dwFrame-tempi,1,fp);
            fclose(fp);

            fp=fopen("temp.jpeg","w+b");
            fwrite(pPicData,dwFrame-tempi,1,fp);
            delete []pPicData;
            DeleteFile(szTempFileName);
            break;
        }
        else
        {
            fseek(fp,-1,SEEK_CUR);
            fread(&jpeg_header_tag1,1,1,fp);
            fseek(fp,0,SEEK_CUR);
            fread(&jpeg_header_tag2,1,1,fp);
            tempi++;
            continue;
        }
    }
    fclose(fp);
}

int main111()
{
    FILE *fp=NULL;

    //ID3大小
    int Len=0;
    fp=OpenMusic(Len);
    if(NULL==fp)
    {
        return 0;
    }

    //图片帧大小
    int dwFrame=0;
    //记录图片标签数据中不是图片数据的字节数
    int tempi=0;
    if(FALSE==GetPicInfo(fp,dwFrame,Len,tempi))
    {
        return 0;
    }

    //获取图片数据
    GetPicRGB(fp,dwFrame,tempi);
    return 1;
}
