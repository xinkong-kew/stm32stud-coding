#include "stm32f10x.h"
#include <stdio.h>
#include "delay.h"

void My_USART_SendBytes(USART_TypeDef *USARTx,uint8_t* pData,uint16_t Size);
void My_USART1_Init(void);
int main(void)
{
	Delay_Init();
	My_USART1_Init();
	
//	printf("Hello World. \r\n");

	while(1)
	{
	uint32_t currenTick  = GetTick();
	
	uint32_t miliSeconds = currenTick %1000;
	currenTick = currenTick / 1000;
	
	uint32_t seconds  = currenTick %60;
	currenTick = currenTick / 60;
	
	uint32_t minutes = currenTick % 60;
	currenTick = currenTick /60;
	
	uint32_t hour = currenTick;
	printf("%02u:%02u:%02u.%03u\r\n",hour,minutes,seconds,miliSeconds);
	
	Delay(100);
	}
}
// @简介：通过串口发送多个字节
// @参数 USARTx：填写串口的名称
// @参数 pData: 要发送的数据
// @参数 Size：要发送数据的数量，单位是字节
void My_USART_SendBytes(USART_TypeDef *USARTx,uint8_t* pData,uint16_t Size)
{
	for(uint32_t i = 0;i< Size;i++)
	{
	// 等待发送数据寄存器为空
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
	
	//写入发送数据寄存器中
	USART_SendData(USART1,pData[i]);
	}
	
	//等待数据发送完成
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
}

//@简介：对USART进行初始化
// 		PB6 - Tx PB7 - Rx
// 		115200，8，1，None,双向
//
void My_USART1_Init(void)
{
//	   //源代码
//	GPIO_InitTypeDef GPIO_InitStruct;
//	// Tx PA9 复用推挽输出
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//模式
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;//最大速度
//	GPIO_Init(GPIOA,&GPIO_InitStruct);
//	
//	//Rx PA10 输入上拉
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;//模式
//	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	
	 //重映射代码
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); //使能AFIO模块的时钟
	GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE); //USART_REMAP = 1;使能重映射
	
	//# 初始化PB6 PB7
	GPIO_InitTypeDef GPIO_InitStruct;
	// Tx PB6 复用推挽输出
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//模式
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;//最大速度
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	//Rx PB7 输入上拉
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;//模式
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	//# UART初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//开启时钟
	USART_InitTypeDef USART_InitStruct;//结构体初始化
	
	USART_InitStruct.USART_BaudRate = 115200;//波特率设置
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//数据模式设置
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;//数据位长度
	USART_InitStruct.USART_StopBits = USART_StopBits_1;  // 停止位长度
	USART_InitStruct.USART_Parity = USART_Parity_No;//无校验 Even偶校验  Odd奇校验
	
	USART_Init(USART1,&USART_InitStruct);

	USART_Cmd(USART1,ENABLE); // 闭合总开关
}

int fputc(int ch,FILE* f)
{
	//#1.等待TDR为空
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
	
	
	//#2.发送
	USART_SendData(USART1,(uint8_t)ch);
}

