#include "stm32f10x.h"
#include "USART.h"
#include "delay.h"

void App_USART1_Init(void);
void App_TIM3_Init(void);
void App_TIM1_Init(void);

int main(void)
{
	App_USART1_Init();
	App_TIM3_Init();
	App_TIM1_Init();
	
	TIM_SetCompare1(TIM3,200);
		while(1)
		{
			//#1.清除Trigger标志位
			TIM_ClearFlag(TIM1,TIM_FLAG_Trigger);
			
			//#2.等待Trigger标志位从0变成1
			while(TIM_GetFlagStatus(TIM1,TIM_FLAG_Trigger) == RESET);
			
			//#3.计算
			uint16_t ccr1 = TIM_GetCapture1(TIM1);//读取输入捕获寄存器的值
			uint16_t ccr2 = TIM_GetCapture2(TIM1);
			
			float Period = ccr1 * 1.0e-6f* 1.0e3f;
			float duty = (float)ccr2 / ccr1* 100.0f;
			
	
			My_USART_Printf(USART1,"周期 = %.3fms,占空比 = %.2f%%\r\n",Period,duty);
			
			Delay(100);
		}
}
void App_USART1_Init(void)
{
	//#1.初始化IO引脚 PA9 AF_PP
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode  =  GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//#2.初始化USART
	//开启USART时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	//配置USART参数
	USART_InitTypeDef USART_InitStruct;
	
	USART_InitStruct.USART_Parity  = USART_Parity_No;//不使用校验
	USART_InitStruct.USART_BaudRate = 115200;//波特率
	USART_InitStruct.USART_StopBits =  1;//停止位
	USART_InitStruct.USART_WordLength = 8; //数据位
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//不使用硬件流控
	USART_InitStruct.USART_Mode =  USART_Mode_Tx;//通信方式
	
	USART_Init(USART1,&USART_InitStruct);
	
	//闭合总开关
	USART_Cmd(USART1,ENABLE);
	
}
void App_TIM3_Init(void)
{
	//#1.初始化时基单元
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	//定义时基单元的参数
	
	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	
	TIM_InitStruct.TIM_Period = 999;//ARR 计数重置
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;//上计数
	TIM_InitStruct.TIM_Prescaler = 71;//分频器系数
	TIM_InitStruct.TIM_RepetitionCounter = 0;//重复计数器
	
	TIM_TimeBaseInit(TIM3,&TIM_InitStruct);
		
	//开启ARR的预加载
	TIM_ARRPreloadConfig(TIM3,ENABLE);
	
	//闭合时基单元的总开关
	TIM_Cmd(TIM3,ENABLE);
	
	//#2.初始化输出比较通道1
	//初始化PA6
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed =  GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//配置OC1的参数 
	TIM_OCInitTypeDef TIM_OCInitStruct = {0};
	
	TIM_OCInitStruct.TIM_OCMode  = TIM_OCMode_PWM1;//模式PWM1
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;//高极性
	TIM_OCInitStruct.TIM_OutputState = ENABLE;//使能
	TIM_OCInitStruct.TIM_Pulse = 0;//CCR寄存器的初始值
	
	TIM_OC1Init(TIM3,&TIM_OCInitStruct);
	
	//使能MOE
	TIM_CtrlPWMOutputs(TIM3,ENABLE);
	
	//#3.使能CCRx的预加载
	TIM_CCPreloadControl(TIM3,ENABLE);
}

void App_TIM1_Init(void)
{
	//#1.配置时基单元
	//1.1开启定时器1的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	
	//1.2配置时基单元的参数
	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	TIM_InitStruct.TIM_Prescaler = 71;//分频器系数
	TIM_InitStruct.TIM_Period = 65535;//计数重置（ARR）
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	TIM_InitStruct.TIM_RepetitionCounter = 0;//重复计数
	
	TIM_TimeBaseInit(TIM1,&TIM_InitStruct);
	
	//1.3开启ARR的预加载
	TIM_ARRPreloadConfig(TIM1,ENABLE);
	//1.4闭合时基单元的总开关
	TIM_Cmd(TIM1,ENABLE);
	
	//#2.初始化输入捕获
	//2.1初始化PA8引脚 IPD
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode  =  GPIO_Mode_IPD;
	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//初始化输入捕获通道1
	TIM_ICInitTypeDef TIM_ICInitStruct;
	
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;//通道
	TIM_ICInitStruct.TIM_ICFilter = 0;//滤波器
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;//捕获边沿
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;//分频器-1分频
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;//选择模式-直接
	
	TIM_ICInit(TIM1,&TIM_ICInitStruct);
	
	//初始化输入捕获通道2
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStruct.TIM_ICFilter = 0;//滤波器
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Falling;//捕获下降沿
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;//分频器-1分频
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_IndirectTI;//模式-间接
	
	TIM_ICInit(TIM1,&TIM_ICInitStruct);
	
	//#3.从模式选择输入来源
	TIM_SelectInputTrigger(TIM1,TIM_TS_TI1FP1);//从模式选择TIM1的TI1FP1作为来源、
	TIM_SelectSlaveMode(TIM1,TIM_SlaveMode_Reset);//选择TRGI（从模式）的模式
	
}

