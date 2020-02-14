/*
 * WaterControl.c
 * 
 * Created: 17.11.2014 22:39:28
 *
 */ 
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "WaterControl.h"
#include "D:\ATMEL_STUDIO6\WaterControl\WaterControl\Library\I2C.h"
#include "D:\ATMEL_STUDIO6\WaterControl\WaterControl\Library\DS1307.h"
#include "D:\ATMEL_STUDIO6\WaterControl\WaterControl\Library\HD44780.h"
#include "D:\ATMEL_STUDIO6\WaterControl\WaterControl\Library\DS18B20.h"

//*********************************************************************************************
//�������� ���������
int main(void)
{
	
	    //SetPinsToOut();
	    //InitScreen();
	    //SendMessageToScreen();
	    //ReadTemperatureFromDS18B20();
		I2C_Init();
		DS1307_Init();
	
	
	
    while(1)
    {
        //TODO:: Please write your application code 
    }
}

//*********************************************************************************************
//���������� �������� I2C
ISR (TWI_vect)
{
	uint8_t ParameterCase;
	
	ParameterCase = (TWSR & (~TWSR_MASK));
	
	switch(ParameterCase)
	{
	//***********************************************
	//C������������ ��������� ����� �� ���� I2C
		case START_CONDITION_I2C1:
		
			TWCR &= ~CLEAR_START_BIT_MASK;			// C������ ��� ������ � �������� ���������� ��������� I2C
			TWDR = (SlaveAddDS1307|0x01);		// �������� ������� ����� + ��� ��������6�� ������ � ������� TWDR
			SEND_BYTE;								// ���� ������� ��������� ������			
		
		break;
		
	//***********************************************
	//������������� ��������� ��������� �����		
		case REPEATED_START_I2C1:
		
			TWCR &= ~CLEAR_START_BIT_MASK;			// C������ ��� ������ � �������� ���������� ��������� I2C
			TWDR = (SlaveAddDS1307|0x00);		// �������� ������� ����� + ��� ��������6�� ������ � ������� TWDR	
			SEND_BYTE;								// ���� ������� ��������� ������
							
		break;

	//***********************************************
	//		
		case ARBITRATION_LOST_I2C1:					
		break;

	//***********************************************
	//����� �������� ���������� + ��� �� ������ ����������		
		case SLADDR_W_RECEIVED_ACK:
			
			TWDR = CurrentAddressRW;		// �������� ������� �����+��� ���������� ������ � ������� TWDR
			SEND_BYTE;								// ���� ������� ��������� ������
			
		break;

	//***********************************************
	//������� ���������� �� ��������, ����� + ��� �� ������ �� �������		
		case SLADDR_W_RECEIVED_NACK:		
		
		break;

	//***********************************************
	//������� ��������� ���� �������� ����������, ����� �������		
		case DATA_TRANSMITTED_ACK_RECEIVED:	
		
			if (RWDeviceFlag == 1)
				{
					SEND_START;
				}
			else
				{
					SendByteI2C();
				}
		
		break;

	//***********************************************
	//������� ���������� �� �������� ������� ������������ ����	
		case DATA_TRANSMITTED_NACK_RECEIVED:
		
		break;

	//***********************************************
	//		
		case SLADDR_R_RECEIVED_ACK:
		
				if (NumberByteRW > 1)
					{
						READ_BYTE_ACK;						// ���� ������� ������� ������, �� �������� ���� ��������� ������������� ACK
					} 
				else
					{
						READ_BYTE;							// ���� ������� ������� ������, �� �������� ������������� NACK
					}
	
		break;

	//***********************************************
	//	
		case SLADDR_R_RECEIVED_NACK:		
		
		break;

	//***********************************************
	//		
		case DATA_RECEIVED_ACK_RETURNED:
		
				ReadByteI2C();
		
		break;

	//***********************************************
	//		
		case DATA_RECEIVED_NACK_RETURNED:	
			
			//������� ���� �������� ����
			SEND_STOP; //������������� ��������� ���� 
						
		break;

	//***********************************************
	//�������� ��� ���������� �� ���������� ���� �����/��������		
		default:
		asm("nop");
		break;
	}
	
	
	
}
