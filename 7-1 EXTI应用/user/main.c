#include "stm32f10x.h"

void App_OnBoardLED_Init(void);
void App_Button_Init(void);
int main(void)
{
	//NVIC模块分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	App_OnBoardLED_Init();
	App_Button_Init();
		while(1)
		{
		}
}
void App_OnBoardLED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Speed =  GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	
	GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);//默认熄灭LED灯
}
void App_Button_Init(void)
{
	//1.初始化PA5和PA6
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//PA5 IPU
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//PA6 IPU
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	// 2.为EXTI5和EXTI6分配引脚
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	//开启EXTI线5和EXTI线6
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIOPinSource5);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIOPinSource6);
	
	//3.初始化EXTI线
	EXTI_InitTypeDef EXTI_InitStruct;
	
	//初始化EXTI5
	EXTI_InitStruct.EXTI_Line = EXTI_Line5;//线的编号
	EXTI_InitStruct.EXTI_Mode =  EXTI_Mode_Interrupt;//线的模式  Interrupt 中断 Event 事件
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;//线探查的边沿 Rising 上升沿 Falling 下降沿
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;//开关闭合  DISABLE 关闭
	
	//初始化EXTI6
	EXTI_InitStruct.EXTI_Line = EXTI_Line6;//线的编号
	EXTI_InitStruct.EXTI_Mode =  EXTI_Mode_Interrupt;//线的模式  Interrupt 中断 Event 事件
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;//线探查的边沿 Rising 上升沿 Falling 下降沿
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;//开关闭合  DISABLE 关闭
	
	EXTI_Init(&EXTI_InitStruct);
	
	//4. 配置中断
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;//中断名称 需要去stm32f10x.h寻找
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;//抢断优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0；//子优先级
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//闭合中断开关
	NVIC_Init(&NVIC_InitStruct);
	}

void EXTI9_5_IRQHandler(void)
{	
	//EXTI_GetFlagStatus(EXTI_Line5) 获取EXTI线5的值
	if(EXTI_GetFlagStatus(EXTI_Line5) == SET)
	{
		EXTI_ClearFlag(EXTI_Line5);//清除中断标志位，向线5写0
		GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET);//亮灯
	}
	//EXTI_GetFlagStatus(EXTI_Line6) 获取EXTI线6的值
		if(EXTI_GetFlagStatus(EXTI_Line6) == SET)
	{
		EXTI_ClearFlag(EXTI_Line6);//清除中断标志位，向线6写0
		GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);//灭灯
	}
	
}
