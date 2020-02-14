/* Created: 03.01.2015 18:27:17
 * I2C.c
 *
 *
 *
 */ 


#include <avr/io.h>
#include <util/delay.h>
#include "I2C.h"
#include "DS1307.h"

uint8_t DirectionData;
uint8_t OffsetAddrRW;
uint8_t NumberByteRW;
uint8_t CurrentAddressRW;
uint8_t RWDeviceFlag;
uint8_t *RWBytePointer;

//*********************************************************************************************
//��������� �������� I2C (������� �������� ������ SCL)
void I2C_Init(void)
{
	TWSR = TWSR_PRESCALER_VALUE;		// ���������� ������������ �������� ������� I2C � �������� TWSR
	TWBR = TWBR_BIT_VALUE;				// ���������� �������� ��� ����� � �������� TWBR
}


//*********************************************************************************************
//��������� ����������� ���������� ����, �� ��������� ������������� ����
void SendByteI2C(void)
{
	uint8_t CounterByte = 0;
	
	if (CounterByte<NumberByteRW)
		{
			RWBytePointer = RWBytePointer + OffsetAddrRW;	//���������� ��������� ����, �������� ��������
			TWDR = (*RWBytePointer);							//�������� ����� ��������� ��������� ����
			OffsetAddrRW++;									//��������� �������� �� 1 (�������� ����� ���������� �����)
			CounterByte++;										//��������� ������� ������������ ���� �� 1
			SEND_BYTE;										//��������� ������� I2C ��� �������� ������
			//return;
		}
	else
		{
			//CounterByte = 0;
			SEND_STOP;										//������������� ��������� STOP
		}

}


//*********************************************************************************************
//������� ����
uint8_t ReadByteI2C(void)
{
		uint8_t CounterByte = 0;
		
		if (CounterByte<NumberByteRW)
		{
			
			OffsetAddrRW++;									//��������� �������� �� 1 (�������� ����� ���������� �����)
			CounterByte++;										//��������� ������� ������������ ���� �� 1
			SEND_BYTE;										//��������� ������� I2C ��� �������� ������
			//return;
		}
		else
		{
			//CounterByte = 0;
			SEND_STOP;										//������������� ��������� STOP
		}
}