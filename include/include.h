#ifndef _INCLUDE_H_
#define _INCLUDE_H_

#include <Arduino.h>
#include "ESP8266WiFi.h"
#include <ArduinoJson.h>
#include "tft.h"
#include <time.h>
#include "fatfs.h"

#include "stdint.h"
#include "stdio.h"
#include "SPI.h"
#include "logo.h"
#include "font.h"
#include "spi_flash.h"

typedef struct{
	struct{
		u8 Dir;
	} Config;
	struct{
		u32 Addr12;
		u32 Addr16;
		u32 Addr32;
	} FontInfo;
}SYS;


#endif
