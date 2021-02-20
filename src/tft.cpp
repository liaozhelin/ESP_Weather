#include "tft.h"
#include "logo.h"

typedef struct{
	struct{
		u8 Dir;
	}Config;
}SYS;

//
SYS Sys;
SPIClass HSPI;

void ICACHE_FLASH_ATTR tft_write_cmd(unsigned char cmd)
{
	HSPI.write_9bit(cmd,0);
}
void ICACHE_FLASH_ATTR tft_write_dat(unsigned char dat)
{
	HSPI.write_9bit(dat,1);
	//spi_lcd_9bit_write(1, 1, dat);
}
void ICACHE_FLASH_ATTR tft_set_window(u16 x1, u16 y1, u16 x2, u16 y2)
{
	tft_write_cmd(0x2a); //--Set MX1_ADDR
	tft_write_dat(x1 >> 8 & 0xff);   //--Set MX1_ADDR
	tft_write_dat(x1);
	tft_write_dat(x2 >> 8 & 0xff);    //Set MX2_ADDR
	tft_write_dat(x2);
	tft_write_cmd(0x2b);  //--Set MY1_ADDR
	tft_write_dat(y1 >> 8 & 0xff);    //--Set MY1_ADDR
	tft_write_dat(y1);
	tft_write_dat(y2 >> 8 & 0xff);    //Set MY2_ADDR
	tft_write_dat(y2);
	tft_write_cmd(0x2C); //дDDRAM
}
void ICACHE_FLASH_ATTR tft_clear(u8 Color)
{
	int i;
	tft_write_cmd(0x3A); 	//Interface Mode Control
	tft_write_dat(0x03); 	// 12 bits / pixel
	tft_set_window(0, 0, 239, 239);
	Color |= (Color << 3);
	for (i = 0; i < 28800; i++)
	{
		tft_write_dat(Color);
		tft_write_dat(Color);
		tft_write_dat(Color);
	}
	tft_write_cmd(0x3A); 	//Interface Mode Control
	tft_write_dat(0x05); 	// 16 bits / pixel
}
void ICACHE_FLASH_ATTR tft_clear_color(u32 Color)
{
	int i, j;
	tft_set_window(0, 240, 0, 240);
	for (i = 0; i < 240; i++)
	{
		//system_soft_wdt_feed();
		for (j = 0; j < 240; j++)
		{
			tft_write_dat(Color >> 16);
			tft_write_dat(Color >> 8);
			tft_write_dat(Color >> 0);
		}
	}
}
void ICACHE_FLASH_ATTR tft_show_frame1(int index)
{
	int i, j;
	u8 buf[5400];
	//u16 color;
	tft_set_window(0, 0, 239, 179);
	//system_soft_wdt_feed();
	//spi_flash_read(0x81000 + index * 5400, (u32*) buf, 5400);
	for (i = 0; i < 5400; i++)
	{
		for (j = 0; j < 8; j++)
		{
			if (buf[i] & 0x80)
			{
				tft_write_dat(0xff);
				tft_write_dat(0xff);
			}
			else
			{
				tft_write_dat(0x00);
				tft_write_dat(0x00);
			}
			buf[i] <<= 1;
		}
	}
}
/*
void ICACHE_FLASH_ATTR tft_show_frame(void)
{
	int i, j;
	u8 buf[960];
	u16 color;
	tft_set_window(0, 0, 239, 239);
	//system_soft_wdt_feed();
	for (i = 0; i < 240; i++)
	{
		spi_flash_read(0x81000 + 54 + i * 720, (u32*) buf, 720);
		for (j = 0; j < 240; j++)
		{
			tft_write_dat(buf[j * 2]);
			tft_write_dat(buf[j * 2 + 1]);
			tft_write_dat(buf[j * 2 + 2]);
		}
	}
}
*/
void lcd_draw_point(u16 x, u16 y, u16 color)
{
	u16 z;
	if (Sys.Config.Dir == 1)
	{
		z = x;
		x = 239 - y;
		y = z;
	}
	else if (Sys.Config.Dir == 2)
	{
		x = 239 - x;
		y = 239 - y;
	}
	else if (Sys.Config.Dir == 3)
	{
		z = x;
		x = y;
		y = 239 - z;
	}
	tft_set_window(x, y, x + 1, y + 1);
	tft_write_dat(color >> 8);
	tft_write_dat(color & 0xff);
}


void ICACHE_FLASH_ATTR tft_show_logo(void)
{
	int i;
	u16 x, y;
	i = 0;
	for (x = 0; x < 64; x++)
	{
		for (y = 0; y < 64; y++)
		{
			lcd_draw_point(x + 88, y + 88,
			gImage_logo[i * 2] * 256 + gImage_logo[i * 2 + 1]);
			i++;
		}
	}
}

void tft_drawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
	u16 t;
	int xerr = 0, yerr = 0, delta_x, delta_y, distance;
	int incx, incy, uRow, uCol;
	delta_x = x2 - x1; //������������
	delta_y = y2 - y1;
	uRow = x1;
	uCol = y1;
	if (delta_x > 0)
		incx = 1; //���õ�������
	else if (delta_x == 0)
		incx = 0; //��ֱ��
	else
	{
		incx = -1;
		delta_x = -delta_x;
	}
	if (delta_y > 0)
		incy = 1;
	else if (delta_y == 0)
		incy = 0; //ˮƽ��
	else
	{
		incy = -1;
		delta_y = -delta_y;
	}
	if (delta_x > delta_y)
		distance = delta_x; //ѡȡ��������������
	else
		distance = delta_y;
	for (t = 0; t <= distance + 1; t++) //�������
	{
		lcd_draw_point(uRow, uCol, color);
		xerr += delta_x;
		yerr += delta_y;
		if (xerr > distance)
		{
			xerr -= distance;
			uRow += incx;
		}
		if (yerr > distance)
		{
			yerr -= distance;
			uCol += incy;
		}
	}
}

void ICACHE_FLASH_ATTR tft_init(void)
{
	HSPI.begin();
    HSPI.setFrequency(80000000);

	delay(120);
	tft_write_cmd(0x11);
	delay(120); //Delay 120ms

	//--------------------------------Display and color format setting-------------------

	tft_write_cmd(0x36);
	tft_write_dat(0x00);
	tft_write_cmd(0x3a);
	tft_write_dat(0x05);
    //--------------------------------ST7789S Frame rate setting-------------------------

	tft_write_cmd(0xb2);
	tft_write_dat(0x0c);
	tft_write_dat(0x0c);
	tft_write_dat(0x00);
	tft_write_dat(0x33);
	tft_write_dat(0x33);
	tft_write_cmd(0xb7);
	tft_write_dat(0x35);
//---------------------------------ST7789S Power setting-----------------------------

	tft_write_cmd(0xbb);
	tft_write_dat(0x35);
	tft_write_cmd(0xc0);
	tft_write_dat(0x2c);
	tft_write_cmd(0xc2);
	tft_write_dat(0x01);
	tft_write_cmd(0xc3);
	tft_write_dat(0x13);
	tft_write_cmd(0xc4);
	tft_write_dat(0x20);
	tft_write_cmd(0xc6);
	tft_write_dat(0x0f);
	tft_write_cmd(0xca);
	tft_write_dat(0x0f);
	tft_write_cmd(0xc8);
	tft_write_dat(0x08);
	tft_write_cmd(0x55);
	tft_write_dat(0x90);
	tft_write_cmd(0xd0);
	tft_write_dat(0xa4);
	tft_write_dat(0xa1);
//--------------------------------ST7789S gamma setting------------------------------
	tft_write_cmd(0xe0);
	tft_write_dat(0xd0);
	tft_write_dat(0x00);
	tft_write_dat(0x06);
	tft_write_dat(0x09);
	tft_write_dat(0x0b);
	tft_write_dat(0x2a);
	tft_write_dat(0x3c);
	tft_write_dat(0x55);
	tft_write_dat(0x4b);
	tft_write_dat(0x08);
	tft_write_dat(0x16);
	tft_write_dat(0x14);
	tft_write_dat(0x19);
	tft_write_dat(0x20);
	tft_write_cmd(0xe1);
	tft_write_dat(0xd0);
	tft_write_dat(0x00);
	tft_write_dat(0x06);
	tft_write_dat(0x09);
	tft_write_dat(0x0b);
	tft_write_dat(0x29);
	tft_write_dat(0x36);
	tft_write_dat(0x54);
	tft_write_dat(0x4b);
	tft_write_dat(0x0d);
	tft_write_dat(0x16);
	tft_write_dat(0x14);
	tft_write_dat(0x21);
	tft_write_dat(0x20);
	tft_write_cmd(0x29);
	tft_write_cmd(0x21);

	tft_clear(0xff);
	tft_show_logo();

}

/*
void tftfonthz(char *code, unsigned char *mat, u8 size)
{
	unsigned char qh, ql;
	unsigned char i;
	unsigned long foffset;
	u8 csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size);
	qh = *code;
	ql = *(++code);
	if (qh < 0x81 || ql < 0x40 || ql == 0xff || qh == 0xff)
	{
		for (i = 0; i < csize; i++)
			*mat++ = 0x00;
		return;
	}
	if (size == 16)
	{
		if (ql < 0x7f)
			ql -= 0x40;
		else
			ql -= 0x41;
		qh -= 0x81;
		foffset = ((unsigned long) 190 * qh + ql) * csize;
	}
	else
	{
		qh -= 0xa1;
		ql -= 0xa1;
		foffset = ((unsigned long) 94 * qh + ql) * csize;
	}
	switch (size)
	{
	case 12:
		spi_flash_read(0x100000 + foffset + Sys.FontInfo.Addr12, (u32*) mat,
		csize);
		break;
	case 16:
		spi_flash_read(0x100000 + foffset + Sys.FontInfo.Addr16, (u32*) mat,
		csize);
		break;
	case 32:
		spi_flash_read(0x100000 + foffset + Sys.FontInfo.Addr32, (u32*) mat,
		csize);
		break;
	}
}

void Show_Font(u16 x, u16 y, char *font, u8 size, u16 color)
{
	u8 temp, t, t1;
	u16 y0 = y;
	u8 dzk[704];
	u8 csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size);
	if (size == 96)
	{
	}
	else
	{
		tftfonthz(font, dzk, size);
		for (t = 0; t < csize; t++)
		{
			temp = dzk[t];
			for (t1 = 0; t1 < 8; t1++)
			{
				if (temp & 0x80)
					lcd_draw_point(x, y, color);
				else
					lcd_draw_point(x, y, 0);
				temp <<= 1;
				y++;
				if ((y - y0) == size)
				{
					y = y0;
					x++;
					break;
				}
			}
		}
	}
}
void lcd_show_char(u16 x, u16 y, u8 size, u8 chrx, u16 color)
{
	u8 temp;
	u8 j;
	u8 pos, t;
	u16 x0 = x;
	if (x > 240 || y > 240)
	{
		return;
	}
	chrx = chrx - ' ';
	for (pos = 0; pos < size; pos++)
	{
		if (size == 8)
		{
			temp = font5x8[chrx][pos];
			j = 5;
		}
		else if (size == 12)
		{
			temp = font6x12[chrx][pos];
			j = 6;
		}
		else if (size == 16)
		{
			temp = font8x16[chrx][pos];
			j = 8;
		}
		else if (size == 32)
		{
			temp = font16x32[chrx][pos * 2];
			for (t = 0; t < 8; t++)
			{
				if (temp & 0x80)
				{
					lcd_draw_point(x, y, color);
				}
				else
				{
					lcd_draw_point(x, y, 0);
				}
				temp <<= 1;
				x++;
			}
			temp = font16x32[chrx][pos * 2 + 1];
			j = 8;
		}
		for (t = 0; t < j; t++)
		{
			if (temp & 0x80)
			{
				lcd_draw_point(x, y, color);
			}
			else
			{
				lcd_draw_point(x, y, 0);
			}
			temp <<= 1;
			x++;
		}
		x = x0;
		y++;
	}
}
void lcd_show_string(u16 x, u16 y, u8 size, char *str, u16 color)
{
	u8 Width;
	u8 bHz = 0;
	while (*str != 0)
	{
		if (!bHz)
		{
			if (*str > 0x80)
			{
				bHz = 1;
			}
			else
			{
				Width = size / 2;
				if (size == 8)
				{
					Width = 5;
				}
				if (x > (240 - Width - 1))
				{
					//x = 0;
					//y += size;
				}
				if (*str == '\r')
				{
					x = 0;
				}
				else if (*str == '\n')
				{
					y += size;
				}
				else
				{
					lcd_show_char(x, y, size, *str, color);
					x += Width;
				}
				str++;
			}
		}
		else
		{
			bHz = 0;
			Show_Font(x, y, str, size, color);
			str += 2;
			x += size;
			if (x > (240 - size))
			{
				//x = 0;
				//y += size;
			}
		}
	}
}
void lcd_show_num(u16 x, u16 y, u8 size, int num, u16 color)
{
	char buf[10];
	cs_itoa(num, buf, 10);
	lcd_show_string(x, y, size, buf, color);
}
void lcd_printf(u16 x, u16 y, u16 size, u16 color, char *fmt, ...)
{
	char buf[128];
	va_list args;
	va_start(args, fmt);
	ets_vsnprintf(buf, sizeof(buf), fmt, args);
	va_end(args);
	lcd_show_string(x, y, size, buf, color);
}
void tftdis_icon(u16 x, u16 y, u16 size, u32 addr)
{
	u8 buf[288];
	u32 i, j;
	u16 dat, tmp;
	y = y + size;
	for (i = 0; i < size; i++)
	{
		spi_flash_read(0x100000 + addr + i * size * 3, (u32*) buf, size * 3);
		for (j = 0; j < size; j++)
		{
			dat = buf[j * 3 + 2];
			dat &= 0xf8;
			dat <<= 8;

			tmp = buf[j * 3 + 1];
			tmp &= 0xfc;
			tmp <<= 3;
			dat |= tmp;

			tmp = buf[j * 3 + 0];
			tmp &= 0xf8;
			tmp >>= 3;
			dat |= tmp;
			lcd_draw_point(x + j, y - i, dat);
		}
	}
}

*/