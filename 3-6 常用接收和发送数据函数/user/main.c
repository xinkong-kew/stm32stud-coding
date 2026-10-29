#include "stm32f10x.h"
#include "usart.h"
void My_USART1_Init(void);


int main(void)
{
	My_USART1_Init();
	
	//发送函数
//	My_USART_SendByte(USART1,0x5a);

//	uint8_t byteArray[] = {1,2,3,4,5};
//	My_USART_SendBytes(USART1,byteArray,5);

//	My_USART_SendChar(USART1,'a');

//	My_USART_SendString(USART1,"hello world");

//	const char *strName = "Tom";
//	My_USART_Printf(USART1,"Hi,%s!Nice to meet you!\r\n",strName);

//	初始化PC13
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;//模式
	
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	while(1)
	{
//		接收函数-一个字符
//		char c = My_USART_ReceiveByte(USART1);
//		
//		if(c == '0')
//		{
//		GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);
//		}
//		else if(c == '1')
//		{
//		GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET);
//		}

// @参数 USARTx       ：串口名称，如USART1, USART2, USART3 ...
// @参数 pStrOut      ：输出参数，读取到的数据将输出到此数组当中
// @参数 MaxLength    ：字符串的最大长度
// @参数 LineSeperator：行分隔符 LINE_SEPERATOR_CR   - 回车 \r
//                               LINE_SEPERATOR_LF   - 换行 \n
//                               LINE_SEPERATOR_CRLF - 回车+换行 \r\n
// @参数 Timeout      ：超时时间，单位是毫秒，负数表示无限长。如果超时时间内没有读取完成则返回
// 
// @返回值：0 - 成功读到一行字符串
//         -1 - 超时（Timeout内未读到一行完整的字符串）
//         -2 - 超过字符串的最大长度（字符串的最大长度用MaxLength参数设置）
		char buffer[100];
		if(My_USART_ReceiveLine(USART1,buffer,100,LINE_SEPERATOR_CRLF,-1)==0)
		{
		My_USART_SendString(USART1,buffer);
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
