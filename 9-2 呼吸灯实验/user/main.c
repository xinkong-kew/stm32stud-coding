#include "stm32f10x.h"
#include "delay.h"
#include "math.h"

void App_PWM_Init(void);

int main(void)
{
	App_PWM_Init();
		while(1)
		{
			float t =GetTick() * 1.0e-3f;//获取当前时间 1.0e-3f等于0.001把ms转换成s  调用delay头文件函数
			float duty = 0.5*(sin(2*3.14*t)+1);//duty存放占空比
			uint16_t ccr1 = duty * 1000;//计算ccr1（捕获比较寄存器）的值
			
			TIM_SetCompare1(TIM1,ccr1);//设置CCR的值
		}
}
void App_PWM_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//#1.初始化引脚
	//PA8	AF_PP 2MHz
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	//PB13 AF_PP
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	//#2.配置时基单元
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	
	//配置时基单元的参数
	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	
	TIM_InitStruct.TIM_Prescaler = 71;//预分频器
	TIM_InitStruct.TIM_Period = 999;//自动重装寄存器（ARR）
	TIM_InitStruct.TIM_RepetitionCounter = 0; //RCR(重复计数器)
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;//计数方向选择上计数
	TIM_TimeBaseInit(TIM1,&TIM_InitStruct);
	
	TIM_ARRPreloadConfig(TIM1,ENABLE);//启动预加载
	
	TIM_Cmd(TIM1,ENABLE);//开启时基单元开关
	
	//#3.初始化输出比较
	//3.1初始化输出比较通道1的参数
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;//输出比较模式选择PWM1
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;//互补输出极性选择正，Low则为负
	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCPolarity_High;//正常输出极性为正
	TIM_OCInitStruct.TIM_OutputNState  =  TIM_OutputNState_Enable;//互补输出开关设置为开
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;//正常输出开关
	TIM_OCInitStruct.TIM_Pulse  =  0; // 输出/捕获比较器初始化为0
	
	TIM_OC1Init(TIM1,&TIM_OCInitStruct);//输出比较通道1初始化
	//3.2 闭合MOE总开关
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
	
	TIM_CCPreloadControl(TIM1,ENABLE);
}

