#include "stm32f10x.h"

void APP_SPI1_Init(void);
//声明数据数据收发函数
void App_SPI_MasterTransmitReceive(SPI_TypeDef* SPIx,const uint8_t *pDataTx,uint8_t *pDataRx,uint16_t Size);


int main(void)
{
	APP_SPI1_Init();
		while(1)
		{
		}
}

void APP_SPI1_Init(void)
{
	//	 1.初始化IO引脚
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SPI1,ENABLE);//使SPI1能重映射
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//PB3 SCK AF_PP 2MHz
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	//PB4 MISO IPU 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	//PB5 MOSI AF_PP
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	//PA15 普通IO Out_PP 2MHz
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
		
	//2.对SPI本身进行初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	
	//SPI参数
	SPI_InitTypeDef SPI_Struct;
	SPI_Struct.SPI_Mode =  SPI_Mode_Master;//Master 主机模式 Slave 从机模式
	SPI_Struct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//方向选择 两线全双工
	SPI_Struct.SPI_DataSize = SPI_DataSize_8b;//数据宽度选择8bit
	SPI_Struct.SPI_CPOL = SPI_CPOL_High;//高极性 Low 低极性
	SPI_Struct.SPI_CPHA =  SPI_CPHA_2Edge;//第二边缘采集 相位
	SPI_Struct.SPI_FirstBit = SPI_FirstBit_MSB;//优先传输最高比特位 LSB优先传输最低比特位
	SPI_Struct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;//分频器设置 64分频
	SPI_Struct.SPI_NSS = SPI_NSS_Soft;//软件NSS Hard 硬件NSS
	
	SPI_Init(SPI1,&SPI_Struct);
	
	SPI_NSSInternalSoftwareConfig(SPI1,SPI_NSSInternalSoft_Set);//内部软NSS输出1
	
}

void App_SPI_MasterTransmitReceive(SPI_TypeDef* SPIx,const uint8_t *pDataTx,uint8_t *pDataRx,uint16_t Size)
{
	SPI_Cmd(SPIx,ENABLE);//1.闭合总开关
	SPI_I2S_SendData(SPIx,pDataTx[0]);//2.发送第一个字节
	//3.传输中间数据，同时接收数据
	for(uint16_t i = 0;i<Size-1;i++)
	{
		//发送一个字节 发送寄存器为空
		while(SPI_I2S_GetFlagStatus(SPIx,SPI_I2S_FLAG_TXE) == RESET);
		SPI_I2S_SendData(SPIx,pDataTx[i+1]);
		//接收一个字节 接收寄存器不为空
		while(SPI_I2S_GetFlagStatus(SPIx,SPI_I2S_FLAG_RXNE) == RESET);
		pDataRx[i] = SPI_I2S_ReceiveData(SPIx);
	}
	//4.读最后一个字节 接收寄存器不为空
		while(SPI_I2S_GetFlagStatus(SPIx,SPI_I2S_FLAG_RXNE) == RESET);
		pDataRx[Size-1] = SPI_I2S_ReceiveData(SPIx);
	//5.断开总开关
	SPI_Cmd(SPIx,DISABLE);
}
