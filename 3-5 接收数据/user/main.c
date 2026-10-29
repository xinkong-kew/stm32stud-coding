#include "stm32f10x.h"

void My_USART1_Init(void);
void My_OnBoardLED_Init(void);


int main(void)
{
	My_USART1_Init();
	My_OnBoardLED_Init();

	while(1)
	{
	//# 1.等待接收数据寄存器非空
	while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == RESET);
	
	//# 2.把数据从接收数据寄存器里读出来
	uint8_t byteRcvd = USART_ReceiveData(USART1);
	
	//# 3.对数据进行出来
	if(byteRcvd == '0')
	{
	GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET); //亮灯
	}
	else if(byteRcvd == '1')
	{
	GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET); //灭灯
	}
	
	}
}


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
void My_OnBoardLED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC,&GPIO_InitStruct);

	GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);//初始熄灭
}
