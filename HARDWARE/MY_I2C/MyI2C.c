#include "MyI2C.h"

void MyI2C_SCL_W_SetState(uint8_t State)
{
	GPIO_WriteBit(I2C_PORT, I2C_SCL, (BitAction)State);
	Tims_delay_us(10);
}

void MyI2C_SDA_W_SetState(uint8_t State)
{
	GPIO_WriteBit(I2C_PORT, I2C_SDA, (BitAction)State);
	Tims_delay_us(10);
}

uint8_t MyI2C_SDA_R_SetState(void)
{
    uint8_t State;
    State=GPIO_ReadInputDataBit(I2C_PORT,I2C_SDA);
    Tims_delay_us(10);
    return State;
}

void MyI2C_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_OD;
    GPIO_InitStruct.GPIO_Pin=I2C_SCL|I2C_SDA;
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(I2C_PORT,&GPIO_InitStruct);

    GPIO_SetBits(I2C_PORT,I2C_SCL|I2C_SDA);
}

void MyI2C_Start(void)
{
    MyI2C_SDA_W_SetState(1);
    MyI2C_SCL_W_SetState(1);
    MyI2C_SDA_W_SetState(0);
    MyI2C_SCL_W_SetState(0);
}

void MyI2C_Stop(void)
{
    MyI2C_SDA_W_SetState(0);
    MyI2C_SCL_W_SetState(1);
    MyI2C_SDA_W_SetState(1);
}

void MyI2C_SendByte(uint8_t Byte)
{
    uint8_t i;
    for ( i = 0; i < 8; i++)
    {
        MyI2C_SDA_W_SetState(Byte&(0x80>>i));
        MyI2C_SCL_W_SetState(1);
        MyI2C_SCL_W_SetState(0);
    }
}

uint8_t MyI2C_ResetByte(void)
{
    uint8_t i,Byte=0x00;
    MyI2C_SDA_W_SetState(1);
    for ( i = 0; i < 8; i++)
    {
        MyI2C_SCL_W_SetState(1);
        if(MyI2C_SDA_R_SetState()==1){Byte|=(0x80>>i);}
        MyI2C_SCL_W_SetState(0);
    }
    return Byte;
}

void MyI2C_SendAck(uint8_t AckBit)
{
    MyI2C_SDA_W_SetState(AckBit);
    MyI2C_SCL_W_SetState(1);
    MyI2C_SCL_W_SetState(0);
}

uint8_t MyI2C_ReceiveAck(void)
{
    uint8_t AckBit;
    MyI2C_SDA_W_SetState(1);
    MyI2C_SCL_W_SetState(1);
    AckBit=MyI2C_SDA_R_SetState();
    MyI2C_SCL_W_SetState(0);
    return AckBit;
}