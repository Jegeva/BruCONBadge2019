#ifndef __BRUCON_NVS_H
#define __BRUCON_NVS_H

#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"

//void bru_nvs_init();

char getBruCONConfigFlag(const char* flag);
char setBruCONConfigFlag(const char* flag,unsigned char flagval);
char setBruCONConfigString(const char * key,char * value);
char getBruCONConfigString(const char * key,char ** value);
char BruCONErase_key(const char* key);
uint32_t getBruCONConfigUint32(const char * flag);
uint32_t setBruCONConfigUint32(const char* flag,uint32_t flagval);
#endif
