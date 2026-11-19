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
			//读取并发送测量结果
			//#1.等待注入序列转换完成
			while(ADC_GetFlagStatus(ADC1,ADC_FLAG_JEOC) == RESET);
			
			//#2.读取转换的结果
			uint16_t  jdr = ADC_GetInjectedConversionValue(ADC1,ADC_InjectedChannel_1);//获取通道JDR1结果
			
			//#3.清除JEOC标志位
			ADC_ClearFlag(ADC1,ADC_FLAG_JEOC);
			
			//#4.结果转换成电压
			float voltage = jdr * (3.3f / 4095);
			
			//#5.通过串口把结果发送出去
			My_USART_Printf(USART1,"%.3f\n",voltage);
			
		}
}

void App_USART1_Init(void)
{
	//#1.初始化IO引脚 PA9 AF_PP
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed =GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//#2.开启USART1的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	//#3.配置串口的参数
	USART_InitTypeDef USART_InitStruct = {0};
	
	USART_InitStruct.USART_BaudRate = 115200;//波特率
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//数据流控-不开启
	USART_InitStruct.USART_Mode = USART_Mode_Tx;//通信方式
	USART_InitStruct.USART_Parity = USART_Parity_No;//奇偶校验 - 不校验
	USART_InitStruct.USART_StopBits = 1;//停止位
	USART_InitStruct.USART_WordLength = 8;//数据位
	
	USART_Init(USART1,&USART_InitStruct);
	
	//#4.闭合总开关
	USART_Cmd(USART1,ENABLE);
}
void App_TIM1_Init(void)
{
	//#1.开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	
	//#2.配置时基单元
	TIM_TimeBaseInitTypeDef TIM_InitStruct = {0};
	
	TIM_InitStruct.TIM_CounterMode =TIM_CounterMode_Up;//向上计数
	TIM_InitStruct.TIM_Period = 999;//计数重置（ARR）-定时周期
	TIM_InitStruct.TIM_Prescaler = 71;//分频器设置71
	TIM_InitStruct.TIM_RepetitionCounter = 0;//重复计数器 - 不重复
	
	TIM_TimeBaseInit(TIM1,&TIM_InitStruct);
	
	//#3.TRGO：Update模式
	TIM_SelectOutputTrigger(TIM1,TIM_TRGOSource_Update);
	
	//#4.开启定时器
	TIM_Cmd(TIM1,ENABLE);
}
void App_ADC_Init(void)
{
	//#1.初始化PA0
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;//模拟模式
	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//#2.配置分频器的分频系数（6分频）
	//配置ADC1的时钟
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	//#3.配置ADC的基本参数
	ADC_InitTypeDef ADC_InitStruct;
	
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;//连续模式关
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;//对齐方式- 右对齐
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//常规序列的外部触发信号选择 - 软件触发
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;//双ADC模式 - 独立模式
	ADC_InitStruct.ADC_NbrOfChannel = 1;  //常规序列通道数
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;//扫描模式开关
	
	ADC_Init(ADC1,&ADC_InitStruct);
	
	//#4.配置注入序列的额外参数
	ADC_InjectedSequencerLengthConfig(ADC1,1);// 注入序列的长度
	
	ADC_ExternalTrigInjectedConvConfig(ADC1,ADC_ExternalTrigInjecConv_T1_TRGO);//配置注入序列外部触发来源
	
	ADC_ExternalTrigInjectedConvCmd(ADC1,ENABLE);//闭合注入序列的开关
	
	ADC_InjectedChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_13Cycles5);//配置注入序列的清单
	
	//#5.闭合ADC的总开关
	ADC_Cmd(ADC1,ENABLE);
}

