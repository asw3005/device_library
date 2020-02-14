/* Created: 25.11.2014 10:22:41
 * DS1307.c
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

//*********************************************************************************************
//������ �� ������ �������� ����������
void ReadDS1307Byte(uint8_t Byte)
{
	
}

//*********************************************************************************************
//������ � ������ ��������
void WriteDS1307Byte(uint8_t Byte)
{
	
}

//*********************************************************************************************
//��������� �������� ���������� � ����� �������� ������ ��� ������ 
void AddressingRTC(uint8_t SlaveAddr, uint8_t Addr, uint8_t ReadWriteBit)
{
	uint8_t Byte = 0;
	
	StartConditionI2C();				//������������ ����� �������	
	Byte = (Addr | ReadWriteBit);			//����������� ���� � ������� ���������� � ��������� ������/������
	SendByteI2C(Byte);					//��������� ����  � ������� ���������� � ��������� ������/������
	ReadACKNACK();							//�������� �������������, ��� ������������ ���� ������ ������� �����������
	SendByteI2C(Addr);					//��������� ����� �������� ����� ������/������
	ReadACKNACK();							//�������� �������������, ��� ������������ ���� ������ ������� �����������
}

//*********************************************************************************************
//��������� ����
void SendByteI2C(uint8_t Byte)
{
	
}  

//*********************************************************************************************
//��������� ����
void ReadByteI2C(uint8_t Byte)
{
	
}


//*********************************************************************************************
//����� ������� �� ����
void StartConditionI2C(void)
{
	//����� �������� ����� ���� ������ ���� ��������
	//���������� ������ ������� ����� ������
	//��������� ����������� ����� ��� �������� ������� �����
}

//*********************************************************************************************
//��������� ����� 
void RStartConditionI2C(void)
{
		//������ ������� ����� �������� ������� ������� ����� ��������� ��� �������� ACK/NACK
	//���������� ������� ������� ����� ������
		//���������� ������� ������� ����� �������� �������
	//���������� ������ ������� ����� ������
		//��������� ����������� ����� ��� �������� ������� ��������� �����
}

//*********************************************************************************************
//���� ������� �� ����
void StopConditionI2C(void)
{
	//���������� ������ ������� ����� ������
		//������ ������� ����� �������� ������� ������� ����� ��������� ��� �������� ACK/NACK
	//���������� ������� ������� ����� �������� �������
		//���������� ������� ������� ����� ������
	//����� ������� ���� ����� ������ �������� ���������	
}

//*********************************************************************************************
//���� ���� ������������� ��� ��������������� �����/�������� ������
uint8_t ReadACKNACK(void)
{
	return(0);
}

//*********************************************************************************************
//���� I2C ���� (�������� ������� ���������� ����� ������ � �������� ��������)
void BusTest(void)
{
	if (!(CLK_PIN & DATA_PIN))
		{
			_delay_ms(1000);
			if (!(CLK_PIN & DATA_PIN))
				{
					//�������� ������ ������������� ���� I2C
						//������������ ��� ����� ��������� ���� �� ������� � ����������� ������ � ����������� �����������
				
				}
			
		}
}



