#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"

void App_USART1_Init(void);
void App_HCSR04_Init(void);

int main(void)
{
	App_USART1_Init();
	App_HCSR04_Init();
	//实现功能
		while(1)
		{
			//#1.清零CNT-向CNT写值
			TIM_SetCounter(TIM1,0);
			
			//#2.清除cc1和cc2标志位-对cc1和cc2清零
			TIM_ClearFlag(TIM1,TIM_FLAG_CC1);
			TIM_ClearFlag(TIM1,TIM_FLAG_CC2);
			
			//#3.开启定时器
			TIM_Cmd(TIM1,ENABLE);
			
			//#4.发送脉冲-向Trig引脚发送10us的脉冲
			GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_SET);
			//延迟10us
			DelayUs(10);
		
			GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_RESET);
			
			//#5.等待cc1和cc2 - 测量完成
			while(TIM_GetFlagStatus(TIM1,TIM_FLAG_CC1) == RESET);
			while(TIM_GetFlagStatus(TIM1,TIM_FLAG_CC2) == RESET);
			
			//#6.关闭定时器 - 计算距离
			
			uint16_t ccr1 = TIM_GetCapture1(TIM1);//获取通道一的值
			uint16_t ccr2 = TIM_GetCapture2(TIM1);//获取通道二的值			
			
			float distance = (ccr2 -  ccr1)  * 1.0e-6f * 340.0f / 2;//计算距离单位米（m） 1.0e-6f = 1 × 10^-6 = 0.000001
			
			My_USART_Printf(USART1,"distance = %4f \r\n",distance);//发送数据到串口
			
			Delay(100);
			
		}
}

void App_USART1_Init(void)
{
	//#1.初始化IO引脚
	//PA9 AF_PP Tx
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	//PA10 IPU Rx
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	//#2.初始化USART1
	//2.1开启USART1的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	//2.2配置参数
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = 115200;//波特率

	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件流控
	USART_InitStruct.USART_Mode  =  USART_Mode_Rx| USART_Mode_Tx;//通信方向
	USART_InitStruct.USART_Parity = USART_Parity_No;//不使用奇偶校验
	USART_InitStruct.USART_StopBits = USART_StopBits_1;//一位停止位
	USART_InitStruct.USART_WordLength =  USART_WordLength_8b; //8位停止位
	
	USART_Init(USART1,&USART_InitStruct);
	
	//2.3闭合总开关
	USART_Cmd(USART1,ENABLE);
	 
}

void App_HCSR04_Init(void)
{
	//#1.初始化时基单元
	//开启定时器1的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	
	//配置时基单元
	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	
	TIM_InitStruct.TIM_CounterMode =TIM_CounterMode_Up;//计数方向-上计数
	TIM_InitStruct.TIM_Prescaler = 71;//预分频器71
	TIM_InitStruct.TIM_Period = 65535;//自动重装寄存器最大值-65535
	TIM_InitStruct.TIM_RepetitionCounter = 0;//重复计数器-0
	
	TIM_TimeBaseInit(TIM1,&TIM_InitStruct);
	
	//#2.初始化输入捕获
	//初始化IO引脚PA8
	//PA8 IPD
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//初始化输入捕获
	//初始化输入捕获的通道1
	TIM_ICInitTypeDef TIM_ICInitStruct;
	
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;//初始化通道名称
	TIM_ICInitStruct.TIM_ICFilter = 0;//输入滤波器 - 不使用
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising; //捕获边沿-上升沿  Falling -下降沿
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;//分频器选择1 一次可触发CCR事件
	TIM_ICInitStruct.TIM_ICSelection =  TIM_ICSelection_DirectTI;//模式选择-直接 IndirectTI-间接

	TIM_ICInit(TIM1,&TIM_ICInitStruct);// 初始化捕获输入的库函数
	//初始化输入捕获的通道2
	
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;//初始化通道名称
	TIM_ICInitStruct.TIM_ICFilter = 0;//输入滤波器 - 不使用
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Falling; //捕获边沿-上升沿  Falling -下降沿
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;//分频器选择1 一次可触发CCR事件
	TIM_ICInitStruct.TIM_ICSelection =  TIM_ICSelection_IndirectTI;//模式选择-直接 IndirectTI-间接

	TIM_ICInit(TIM1,&TIM_ICInitStruct);// 初始化捕获输入的库函数
	
	//#3.初始化Trig引脚
	//PA0 OUT-PP
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;//通用输出推挽
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
}

