#include "stm32f10x.h"

int main(void)
{
	//初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//开启时钟
	USART_InitTypeDef USART_InitStruct;//结构体初始化
	
	USART_InitStruct.USART_BaudRate = 115200;//波特率设置
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//数据模式设置
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;//数据位长度
	USART_InitStruct.USART_StopBits = USART_StopBits_1;  // 停止位长度
	USART_InitStruct.USART_Parity = USART_Parity_No;//无校验 Even偶校验  Odd奇校验
	
	USART_Init(USART1,&USART_InitStruct);
	
		while(1)
		{
		}
}
