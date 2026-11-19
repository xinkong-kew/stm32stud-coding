#include "stm32f10x.h"
#include "usart.h"

void App_USART1_Init(void);
void App_TIM1_Init(void);
void App_ADC_Init(void);

int main(void)
{
	App_USART1_Init();
	App_TIM1_Init();
	App_ADC_Init();
		while(1)
		{
			//#1.等待注入序列转换完成 - JEOC
			while(ADC_GetFlagStatus(ADC1,ADC_FLAG_JEOC) == RESET);
			
			//#2.读出转换结果
			uint16_t jdr1 = ADC_GetInjectedConversionValue(ADC1,ADC_InjectedChannel_1);
			uint16_t  jdr2 = ADC_GetInjectedConversionValue(ADC1,ADC_InjectedChannel_2);
			
			//#3.计算电压
			float v1,v2;
			v1 = jdr1 * (3.3f/4095);
			v2 = jdr2 * (3.3f/4095);
			
			//#4.把结果通过串口发送出去
			My_USART_Printf(USART1,"%.3f,%.3f\n",v1,v2);
		}
}

void App_USART1_Init(void)
{
	//#1，初始化PA9 AF_PP
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//#2.初始化USART参数
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	USART_InitTypeDef USART_InitStruct;
	
	USART_InitStruct.USART_BaudRate = 115200;//波特率 
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无数据流控
	USART_InitStruct.USART_Mode = USART_Mode_Tx;//通信方式 - 发数据
	USART_InitStruct.USART_Parity = USART_Parity_No;//奇偶校验无
	USART_InitStruct.USART_StopBits = USART_StopBits_1;//停止位 - 1
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;//数据位 - 8
	
	USART_Init(USART1,&USART_InitStruct);
	
	//#3.闭合总开关
	USART_Cmd(USART1,ENABLE);
}

void App_TIM1_Init(void)
{
	//#1.开启定时器1的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	
	//#2.配置时基单元的参数
	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	TIM_InitStruct.TIM_Period = 71;//分频器系数 - 71
	TIM_InitStruct.TIM_Prescaler = 999;//重置计数（ARR） - 999
	TIM_InitStruct.TIM_RepetitionCounter = 0;//重复计数次数(RCR) - 0；
	
	TIM_TimeBaseInit(TIM1,&TIM_InitStruct);
	
	//#3.开启ARR的预加载
	TIM_ARRPreloadConfig(TIM1,ENABLE);
	
	//#4.配置从模式控制器-将TRGO设置为Update模式
	TIM_SelectOutputTrigger(TIM1,TIM_TRGOSource_Update);
	
	//#5.闭合TIM1的总开关
	TIM_Cmd(TIM1,ENABLE);
}

void App_ADC_Init(void)
{
	//#1.初始化IO引脚 PA0 PA1 -- AIN
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	//PA0
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	//PA1
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//#2.设置ADC分频器系数-6分频
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	//开启ADC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	//#3.初始化ADC1的基本参数
	ADC_InitTypeDef ADC_InitStruct;
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;//连续模式
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;//对齐方式- 右对齐
	//常规序列外部触发方式-由于用的注入序列，随便写的软件触发
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;//双ADC模式 - 独立模式
	ADC_InitStruct.ADC_NbrOfChannel = 1;//常规序列通道数量 - 清单数量1
	ADC_InitStruct.ADC_ScanConvMode = ENABLE;//扫描模式开
	
	ADC_Init(ADC1,&ADC_InitStruct);
	
	//#4.配置注入序列参数
	ADC_InjectedSequencerLengthConfig(ADC1,2);//注入序列的长度 - 2个通道
	//注入序列清单设置        ADC名称   通道编号  写到第几行    周期
	ADC_InjectedChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_13Cycles5);
	ADC_InjectedChannelConfig(ADC1,ADC_Channel_1,2,ADC_SampleTime_13Cycles5);
	//选择注入序列的外部触发  - 选择定时器1的TRGO
	ADC_ExternalTrigInjectedConvConfig(ADC1,ADC_ExternalTrigInjecConv_T1_TRGO);
	//闭合注入序列外部触发的开关
	ADC_ExternalTrigInjectedConvCmd(ADC1,ENABLE);
	
	//#5. 闭合ADC的总开关
	ADC_Cmd(ADC1,ENABLE);
	
}

