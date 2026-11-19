#include "stm32f10x.h"

void SystemClock_Init(void);//时钟树初始化 注释掉启动文件
int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//开启GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//开启USART2时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);//开启DMA1时钟
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1,ENABLE);//施加复位信号
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1,ENABLE);//释放复位信号
	
	SystemClock_Init();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC,&GPIO_InitStruct);
		while(1)
		{
			GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET);
			
			for(uint32_t i = 0;i<666666;i++);//延迟500ms
			
			GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);
			for(uint32_t i = 0;i<666666;i++);//延迟500ms 严格
		}
}
void SystemClock_Init(void)
{
	//开启Flash指令预取（需要在SYSCLK <= 8MHz时进行）
	FLASH_PrefetchBufferCmd(ENABLE);
	//开启访问延迟
	FLASH_SetLatency(FLASH_Latency_2);
	//#1. 开启HSE
	RCC_HSEConfig(RCC_HSE_ON);//开启 RCC_HSE_OFF 关闭
	while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);//等待HSE就绪
	
	//#2.配置并启动锁相环
	//锁相环选择HSE输入
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);//选择分频器*9
	RCC_PLLCmd(ENABLE);//开启PLL
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);//等待PLL（锁相环）就绪
	//#3.配置AHB APB1 和APB2分频器的分频系数
	RCC_HCLKConfig(RCC_SYSCLK_Div1);//HCLK = SYSCLK / 1 AHB分频器
	RCC_PCLK1Config(RCC_SYSCLK_Div2);//PCLK1 = HCLK / 2 APB1分频器
	RCC_PCLK2Config(RCC_SYSCLK_Div1);//PCLK2 = HCLK / 1 APB2分频器
	
	//#4. 切换SYSCLK的来源
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);//SYSCLK来源于锁相环
	while(RCC_GetSYSCLKSource() != 0x08);//等待切换完成 0x08-锁相环	
	
}

