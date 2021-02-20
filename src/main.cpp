#include <Arduino.h>
#include "ESP8266WiFi.h"
#include <ArduinoJson.h>
#include "tft.h"

const unsigned long BAUD_RATE = 9600;                   // serial connection speed
const char* ssid="501 Master 2.4Ghz";
const char* password="1999052912";
const char *buf = "hello world";
/*
void spi_lcd_9bit_write(uint8 spi_no,uint8 high_bit,uint8 low_8bit)
{
	uint32 regvalue;
	uint8 bytetemp;
	if(spi_no>1) 		return; //handle invalid input number
	
	if(high_bit)		bytetemp=(low_8bit>>1)|0x80;
	else				bytetemp=(low_8bit>>1)&0x7f;

	regvalue= ((8&0x0000000F)<<28)|((uint32)bytetemp);

	regvalue= ((8&SPI_USR_COMMAND_BITLEN)<<SPI_USR_COMMAND_BITLEN_S)|((uint32)bytetemp);		//configure transmission variable,9bit transmission length and first 8 command bit 
	if(low_8bit&0x01) 	regvalue|=BIT15;        //write the 9th bit
	while(READ_PERI_REG(SPI_CMD(spi_no))&SPI_USR);		//waiting for spi module available
	WRITE_PERI_REG(SPI_USER2(spi_no), regvalue);				//write  command and command length into spi reg
	SET_PERI_REG_MASK(SPI_CMD(spi_no), SPI_USR);		//transmission start
}
*/
void setup() {
  Serial.begin(BAUD_RATE);
  tft_init();
  WiFi.mode(WIFI_STA);
  while (WiFi.status() != WL_CONNECTED){
     delay(100);
  }
  tft_clear(0x00);
  
  tft_show_bmp(0, 112, 64, (uint8_t*)gImage_CLOUD_1);
  tft_show_bmp(0, 176, 64, (uint8_t*)gImage_CLOUD_2);
  tft_show_bmp(64, 112, 64, (uint8_t*)gImage_RAIN_1);
  tft_show_bmp(64, 176, 64, (uint8_t*)gImage_RAIN_2);
  
  //tft_show_bmp(128, 112, 64, (uint8_t*)gImage_RAIN_3);
  //tft_show_bmp(128, 176, 64, (uint8_t*)gImage_RAIN_4);
  //wdt_enable(10000);
  //wdt_reset();
  //const uint32_t mask = ~((SPIMMOSI << SPILMOSI) | (SPIMMISO << SPILMISO));
  //SPI1U1 = ((SPI1U1 & mask) | ((8 << SPILMOSI) | (8 << SPILMISO)));
  //tft_init();
  
  // put your setup code here, to run once:
}

void loop() {
  static int i;
  lcd_show_char(0, 0, 12, 'A',0Xffff);
  lcd_show_string(0, 12, 16, (char *)buf, 0Xffff);
  lcd_show_num(0, 28, 32,i, 0Xffff);
  lcd_show_string(0, 60, 32, (char *)buf, 0Xffff);




  //lcd_printf(0,60,32,0Xffff,"HELLO WORLD");
  i = (i<65535?i+1:0);

  /*
  u8 i;
  wdt_disable();
  for (i = 0; i < 240;i++){
      tft_drawLine(i, 0, 239-i, 239, 0xFFE0);
      
  }
  WDT_FEED();
  for (i = 0; i < 240;i++){
      tft_drawLine(0, i, 239, 239-i, 0xF800);
      
  }
  WDT_FEED();
  wdt_enable(65535);
  tft_clear(0x00);
  */

  //tft_clear_color(0xF81F);
  // reset to 8Bit mode
  //lcd_draw_point(20,20,0xffea);
  // while(SPI1CMD & SPIBUSY) {}
  // SPI1W0 = 0B11111111;
  // SPI1CMD |= SPIBUSY;
  // SPI1W0 = 0B11111111;
  // while(SPI1CMD & SPIBUSY) {}
  //HSPI.transfer16(0XAAAA);
  //HSPI.transferBytes(&RX_Buffer,&TX_Buffer,9);
  // put your main code here, to run repeatedly:
}