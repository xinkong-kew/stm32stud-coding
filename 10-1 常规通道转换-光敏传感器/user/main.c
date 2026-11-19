#include "stm32f10x.h"

void App_ADC1_Init(void);
void App_OnboardLED(void);

int main(void)
{
	App_ADC1_Init();
	App_OnboardLED();
		while(1)
		{
			//#1.清除EOC标志位
			ADC_ClearFlag(ADC1,ADC_FLAG_EOC);
			
			//#2.软件启动方式发送脉冲
			ADC_SoftwareStartConvCmd(ADC1,ENABLE);
			
			//#3.等待常规序列转换完成
			while (ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == RESET);
			
			//#4.读取DR寄存器的值
			uint16_t dr = ADC_GetConversionValue(ADC1);
			
			//#5.转换成电压值
			float U = dr* (3.3f/4095);//采样深度为12位，所以最大为4095			
			
			//电压控制板载LED
			if(U > 1.5)
			{
				GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET);//点亮
			}
			else
			
			{	
				GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);//熄灭
			}
		}
}

void App_ADC1_Init(void)
{
	//#1.初始化PA0引脚 模拟模式
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;//模拟模式
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//#2.配置ADC模块时钟
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); //ADC模块的分频器系数-六分频
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	//#3.初始化ADC的基本参数
	ADC_InitTypeDef ADC_InitStruct = {0};
	
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE; //连续模式的开关
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;//对齐方式（左/右） -右对齐
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//外部触发设置 - 软件启动 
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;//双ADC模式设置 - 独立模式
	ADC_InitStruct.ADC_NbrOfChannel =1; //设置常规序列的通道数 - 常规序列一个通道
	ADC_InitStruct.ADC_ScanConvMode = DISABLE; //扫描模式的开关	-关闭
	
	ADC_Init(ADC1,&ADC_InitStruct);
	
	//#4.配置常规序列
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_13Cycles5);//通道0，第一列，13.5倍周期-与10.24周期数接近
	ADC_ExternalTrigConvCmd(ADC1,ENABLE);//外部触发开关
	
	
	//#5.闭合ADC的总开关
	ADC_Cmd(ADC1,ENABLE);
}

void App_OnboardLED(void)
{
	//初始化PC13
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;//输出开漏模式
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	
	GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);//初始熄灭
}
	

