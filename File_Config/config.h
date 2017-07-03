#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <SPI.h>
#include <SD.h>

int32_t config_init(void);
int32_t config_read(const char* name, char* value, int* len);
int32_t config_write(const char* name, char* value, int len);
int32_t config_dump(void);

#endif//__CONFIG_H__
