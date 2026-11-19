#include "stm32f10x.h"

volatile uint32_t currentTick = 0;// 用来记录当前时间，单位是ms

void App_Delay(uint32_t ms);
void App_TIM3_TimebaseInit(void);
void OnBoardLED(void);


int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//NVIC分组
	App_TIM3_TimebaseInit();
	OnBoardLED();
		while(1)
		{
			GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET);
			App_Delay(50);
			GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);
			App_Delay(500);
			
			GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET);
			App_Delay(50);
			GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);
			App_Delay(500);
			
			GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET);
			App_Delay(500);
			GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);
			App_Delay(500);
		}
}


void App_Delay(uint32_t ms)
{
	uint32_t expireTime = currentTick + ms;
	while(currentTick < expireTime);
}

void App_TIM3_TimebaseInit(void)	
{
	//1.开启定时器3的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	//2. 配置时基单元的参数
	TIM_TimeBaseInitTypeDef Tim_InitStruct;
	
	Tim_InitStruct.TIM_Prescaler = 71;//预分频器(PSC)设置为71
	Tim_InitStruct.TIM_Period = 999; //自动重装寄存器(ARR)设置为999
	Tim_InitStruct.TIM_CounterMode = TIM_CounterMode_Up; //计数器(CNT)设置为上计数
	Tim_InitStruct.TIM_RepetitionCounter = 0;//重复计数器(RCR)设置为0
	
	TIM_TimeBaseInit(TIM3,&Tim_InitStruct);//初始化
	
	//3.闭合时基单元开关
	TIM_Cmd(TIM3,ENABLE);
	
	//4.使能Update中断
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	
	//5.配置NVIC
	NVIC_InitTypeDef NVIC_InitStruct;
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;//中断名称 去头文件中找
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;//子优先级
	NVIC_InitStruct.NVIC_IRQChannelCmd  = ENABLE;//使能
	
	NVIC_Init(&NVIC_InitStruct);
	
}
 
//中断响应函数
void TIM3_IRQHandler(void)
{
	if(TIM_GetFlagStatus(TIM3,TIM_FLAG_Update) == SET)//判断是否是Update触发中断
	{
		TIM_ClearFlag(TIM3,TIM_FLAG_Update);//清除Update标志位
		currentTick++;//当前值加一
	}
	
}

void OnBoardLED(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Speed  = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	
	GPIO_WriteBit (GPIOC,GPIO_Pin_13,Bit_SET);
}

