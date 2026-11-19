#include "stm32f10x.h"
#include "si2c.h"
#include "oled.h"
#include "hyjk16.h"

SI2C_TypeDef si2c;
OLED_TypeDef oled;
//uint8_t bitmap[] = {};

void My_SoftwareI2C_Init(void);
void My_OLEDScreen_Init(void);


int i2c_write_bytes(uint8_t addr,const uint8_t *pdata,uint16_t size);

int main(void)
{
	My_SoftwareI2C_Init();//软I2C串口初始化
	My_OLEDScreen_Init();//OLED显示器初始化
	// 1.打印Hello World
	
//	OLED_SetPen(&oled,PEN_COLOR_WHITE,1); //设置白色画笔
//	OLED_SetBrush(&oled,PEN_COLOR_TRANSPARENT); //设置笔刷
//	OLED_SetCursor(&oled,24,50); //设置光标
//	OLED_DrawString(&oled,"Hello world");//打印字符串
//	OLED_SendBuffer(&oled);//发送到屏幕上
	
	//2 .打印你好世界
	OLED_SetFont(&oled,&hyjk16);
	//计算横坐标
	int16_t x = (OLED_GetScreenWidth(&oled) - OLED_GetStrWidth(&oled,"你好世界")) / 2;
	OLED_SetCursor(&oled,x,28);
	OLED_DrawString(&oled,"你好世界");
	OLED_SendBuffer(&oled);//发送到屏幕上

	//3.格式化打印当前日期
//	OLED_SetFont(&oled,&default_font);//还原成默认字体
//	OLED_SetCursor(&oled,58,64); //光标
//	OLED_Printf(&oled,"%04d/%02d/%02d",2026,10,30);
//	
//	OLED_SendBuffer(&oled);//发送到屏幕上

	//4.设置文本区域
//	OLED_StartTextRegion(&oled,0,0,128,64);//设置文本框区域
//	OLED_DrawString(&oled,"There was a man named John.\r\n");
//	OLED_DrawString(&oled,"He was 70 years old.\r\n");
//	OLED_DrawString(&oled,"He really liked his name of John.\r\n");
//	OLED_DrawString(&oled,"But didn't like the age of 70.\r\n");
//	OLED_SendBuffer(&oled);//发送到屏幕上
	
	//5.绘图
	//画点
//	OLED_SetPen(&oled,PEN_COLOR_WHITE,3);
//	OELD_SetCursor(&oled,29,32);
//	OLED_DrawDot(&oled);//画第一个点
//	for(int8_t i=1;i<8;i++)
//	{
//		OLED_MoveCursorX(&oled,10);//每次移动10距离
//		OLED_DrawDot(&oled);
//	}
//	OLED_SendBuffer(&oled);//发送到屏幕上

	//画线
	//第一条线
//	OLED_SetCursor(&oled,0,0);
//	OLED_DrawLine(&oled,128,64);//画线
//	
//	//第二条线
//	OLED_SetCursor(&oled,0,64);
//	OLED_DrawLine(&oled,128,0);
//	
//	OLED_SetCursor(&oled,84,22);
//	OLED_LineTo(&oled,44,22);//画线，光标移动到线的末端
//	OLED_LineTo(&oled,44,42);
//	OLED_LineTo(&oled,84,42);

//	OLED_SendBuffer(&oled);//发送到屏幕上

//	画矩形和圆
//	OLED_SetCursor(&oled,20,20);
//	OLED_SetPen(&oled,PEN_COLOR_WHITE,1);
//	OLED_SetBrush(&oled,BRUSH_TRANSPARENT);//透明画刷
//	OLED_DrawRect(&oled,40,20);//画矩形，设置长和宽
//	
//	OLED_SetCursor(&oled,65,30);
//	OLED_DrawCircle(&oled,5);//画圆，设置半径
//	OLED_SetCursor(&oled,70,20);
//	OLED_SetPen(&oled,PEN_COLOR_TRANSPARENT,1);//透明画笔
//	OLED_SetBrush(&oled,BRUSH_WHITE);
//	OLED_DrawRect(&oled,40,20);//第二个矩形，透明背景
//	
//	OLED_SendBuffer(&oled);
//	
//	//6.绘制图像
//	OLED_SetCursor(&oled,0,0);
//	OLED_DrawBitmap(&oeld,128,64,bitmap);
//	
//	OLED_SendBuffer(&oled);
	
	while(1)
		{
		}
}
//定义软串口pb6 pb7
void My_SoftwareI2C_Init(void)
{
	si2c.SCL_GPIOx = GPIOB;
	si2c.SCL_GPIO_Pin = GPIO_Pin_6;
	si2c.SCL_GPIOx = GPIOB;
	si2c.SCL_GPIO_Pin = GPIO_Pin_7;
	
	My_SI2C_Init(&si2c);
}

//回调函数
int i2c_write_bytes(uint8_t addr,const uint8_t *pdata,uint16_t size)
{
	return My_SI2C_SendBytes(&si2c,addr,pdata,size);
}

void My_OLEDScreen_Init(void)
{
	OLED_InitTypeDef OLED_InitStruct;
	
	OLED_InitStruct.i2c_write_cb = i2c_write_bytes;
	
	OLED_Init(&oled,&OLED_InitStruct);
}

