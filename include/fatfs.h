#ifndef _FATFS_H_
#define _FSTFA_H_

#include <Arduino.h>
#include <FS.h>
#include <LittleFS.h>

typedef struct
{
    uint8_t fontok; //字库存在标志，0XAA，字库正常；其他，字库不存在
    uint32_t ugbkaddr; //unigbk 的地址
    uint32_t ugbksize; //unigbk 的大小
    uint32_t f16addr; //gbk16 地址
    uint32_t gbk16size; //gbk16 的大小
    uint32_t f32addr; //gbk24 地址
    uint32_t gkb32size; //gbk24 的大小
}_font_info;

void FS_INIT(void);
void listDir(const char *dirname);
void readFile(const char *path);
void writeFile(const char *path, const char *message);
void appendFile(const char *path, const char *message);
void renameFile(const char *path1, const char *path2);
void deleteFile(const char *path);



#endif