#ifndef GETALBUMPICTURE
#define GETALBUMPICTURE

#include <QString>

/**
 * @brief getAlbumPicture 获取mp3中的IDV3标签的专辑图片
 * @param audioName 歌曲名
 * @return 专辑图片在内存中的二进制数据
 */
char * getAlbumPicture(const QString &audioName);

#endif // GETALBUMPICTURE

