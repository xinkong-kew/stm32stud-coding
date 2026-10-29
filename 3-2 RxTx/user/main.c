#include "stm32f10x.h"

void My_USART_SendBytes(USART_TypeDef *USARTx,uint8_t* pData,uint16_t Size);

int main(void)
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
	
		//UART初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//开启时钟
	USART_InitTypeDef USART_InitStruct;//结构体初始化
	
	USART_InitStruct.USART_BaudRate = 115200;//波特率设置
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//数据模式设置
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;//数据位长度
	USART_InitStruct.USART_StopBits = USART_StopBits_1;  // 停止位长度
	USART_InitStruct.USART_Parity = USART_Parity_No;//无校验 Even偶校验  Odd奇校验
	
	USART_Init(USART1,&USART_InitStruct);

	USART_Cmd(USART1,ENABLE); // 闭合总开关
	uint8_t bytestToSend[] = {1,2,3,4,5};
	
	My_USART_SendBytes(USART1,bytestToSend,5);
	while(1)
	{
	}
}
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

