#include "stm32f10x.h"

void APP_SPI1_Init(void);
void App_W25Q64_SaveByte(uint8_t  Byte);//使用W25Q64保存一个字节
uint8_t App_W25Q64_LoadByte(void);//把保存的字节读出来

uint8_t buffer[10];
uint8_t a = 0;

//声明数据数据收发函数
void App_SPI_MasterTransmitReceive(SPI_TypeDef* SPIx,const uint8_t *pDataTx,uint8_t *pDataRx,uint16_t Size);


int main(void)
{
	APP_SPI1_Init();
	
	App_W25Q64_SaveByte(0x12);//保存一个0x12数据
	
	a = App_W25Q64_LoadByte();//读出数据
	
		while(1)
		{
		}
}

void APP_SPI1_Init(void)
{
	//	 1.初始化IO引脚
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SPI1,ENABLE);//使SPI1能重映射
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//使PA15重映射成普通的PA15接口，默认是调试接口-JTD1
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
	GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_SET);
		
	//2.对SPI本身进行初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	
	//SPI参数
	SPI_InitTypeDef SPI_Struct;
	SPI_Struct.SPI_Mode =  SPI_Mode_Master;//Master 主机模式 Slave 从机模式
	SPI_Struct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//方向选择 两线全双工
	SPI_Struct.SPI_DataSize = SPI_DataSize_8b;//数据宽度选择8bit
	SPI_Struct.SPI_CPOL = SPI_CPOL_Low;//High 高极性 Low 低极性
	SPI_Struct.SPI_CPHA =  SPI_CPHA_1Edge;//第一边缘采集 相位
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

void App_W25Q64_SaveByte(uint8_t  Byte)//使用W25Q64保存一个字节
{
	
	
	//#1.写使能
	buffer[0] = 0x16; // 使能指令  主机发送0x16
	GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_RESET);//选中
	
	App_SPI_MasterTransmitReceive(SPI1,buffer,buffer,1);//发数据
	
	GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_RESET);//取消选中

	//#2.扇区擦除
	buffer[0] = 0x20;//扇区擦除指令 主机发0x20 + 24位地址
	buffer[1] = 0x00;
	buffer[2] = 0x00;
	buffer[3] = 0x00;
	GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_RESET);//选中
	
	App_SPI_MasterTransmitReceive(SPI1,buffer,buffer,4);//发数据
	
	GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_RESET);//取消选中
	
	//#3.等待空闲
	while(1)
	{
		GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_RESET);//选中
		
		//写0x05-指令码等待空闲
		buffer[0]  = 0x05;
		App_SPI_MasterTransmitReceive(SPI1,buffer,buffer,1);//发数据
		
		//读状态寄存器1
		buffer[0]  = 0xff;
		App_SPI_MasterTransmitReceive(SPI1,buffer,buffer,1);//收数据
		
		GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_RESET);//取消选中
		
		if(buffer[0] == 0) break;
	}
		
	//#4.写使能
	buffer[0] = 0x16; // 使能指令  主机发送0x16
	GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_RESET);//选中
	
	App_SPI_MasterTransmitReceive(SPI1,buffer,buffer,1);//发数据
	
	GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_RESET);//取消选中
		
	//#5.页编程
	buffer[0] = 0x02;//页编程指令 主机发0x20 + 24位地址+要写的数据
	buffer[1] = 0x00;//24位地址
	buffer[2] = 0x00;
	buffer[3] = 0x00;
	buffer[4] = Byte;//要写的数据
	GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_RESET);//选中
	
	App_SPI_MasterTransmitReceive(SPI1,buffer,buffer,5);//发数据
	
	GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_RESET);//取消选中
	//#6.等待空闲
	while(1)
	{
		GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_RESET);//选中
		
		//写0x05-指令码等待空闲
		buffer[0]  = 0x05;
		App_SPI_MasterTransmitReceive(SPI1,buffer,buffer,1);//发数据
		
		//读状态寄存器1
		buffer[0]  = 0xff;
		App_SPI_MasterTransmitReceive(SPI1,buffer,buffer,1);//收数据
		
		GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_RESET);//取消选中
		
		if(buffer[0] == 0) break;
	}
	
}
uint8_t App_W25Q64_LoadByte(void)//把保存的字节读出来
{
	//读取一个字节
	buffer[0] = 0x03;//读取字节指令码 主机发0x03 + 24位地址
	buffer[1] = 0x00;
	buffer[2] = 0x00;
	buffer[3] = 0x00;
	GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_RESET);//选中
	
	App_SPI_MasterTransmitReceive(SPI1,buffer,buffer,4);//发数据
	
	buffer[0] = 0xff;
	App_SPI_MasterTransmitReceive(SPI1,buffer,buffer,1);//收一个字节
	
	GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_RESET);//取消选中
	
	return buffer[0];
}

