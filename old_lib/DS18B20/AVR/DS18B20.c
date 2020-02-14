/*
 * DS1821.c
 *
 *
 *
 */ 
#include <avr/io.h>
#include <util/delay.h>
#include "DS18B20.h"

uint8_t ScratchPadBuff[9];
uint8_t ThausendCount, HundredCount, TensCount, UnitsCount = 0;

//******************************************************************************************************************
//��������� ����������� � �������
int16_t ReadTemperatureFromDS18B20(void)
{
	int16_t Temperature = 0;
	uint16_t SignBit = 0;
	uint8_t i;
	
	Reset1WireLine();									//����� ��������� �� ����� � ���� ������� ����������
	WriteByteToDS18B20(SKIP_ROM);							//���������� ���������
	WriteByteToDS18B20(START_CONVERT_T);				//��������� �������������� �����������
	//� ���� ������ �������� ����� � �������, ���� ������������ ���������� �������
	_delay_ms(1000);										//��������� ��������� �������������� (�� ����� ������)
	Reset1WireLine();									//����� ��������� �� ����� � ���� ������� ����������
	WriteByteToDS18B20(SKIP_ROM);							//���������� ���������
	WriteByteToDS18B20(READ_SCRATCHPAD);				//�������� ������ ������, �������� 9 ����, ������ 2 ����� ������� � ������� ����� �����������
	for (i=0; i<2; i++)
		{
			ScratchPadBuff[i] = ReadByteFrom1Wire();		//���������� ������, 9 ����
		}

	Temperature |= ScratchPadBuff[1];					//���������� ������� ���� �����������
	Temperature = Temperature<<8;							//�������� ���� �� ��� �����, �.�. ��� ����������� ��������� �� ����� �������� �����					
	Temperature |= ScratchPadBuff[0];					//���������� ������� ���� �����������
	SignBit = ((~(0x7FFF)) & Temperature);					//��������� ���� �����������
	Temperature = ( (~(0xFF00)) & (Temperature>>3));	//�������� ���� �� 3 �������, �.�. ������������ 9-�� ������ ����� ��������� �����������, ������� ������� 8 ���
	Temperature = Temperature*5;

	BCDtoDEC(Temperature);									//���������� ������, �����, ������� � �������
	//Temperature = Temperature/10;
	
	
	Temperature |= SignBit;

	return(Temperature);	
}


//******************************************************************************************************************
//C������ �����, �����, �������� � ������
void BCDtoDEC(int16_t BCDNumber)
{
	ThausendCount = 0;
	HundredCount = 0;
	TensCount = 0;
	UnitsCount = 0;
	//asm("nop");
	
	BCDNumber = BCDNumber-1000;
	while (BCDNumber>=0)
	{
		ThausendCount++;
		BCDNumber = BCDNumber-1000;
	}
	
	
	BCDNumber = BCDNumber+1000;
	//asm("nop");
	BCDNumber = BCDNumber-100;
	while (BCDNumber>=0)
	{
		HundredCount++;
		BCDNumber = BCDNumber-100;
	}
	
	
	BCDNumber = BCDNumber+100;
	//asm("nop");
	BCDNumber = BCDNumber-10;
	while (BCDNumber>=0)
	{
		TensCount++;
		BCDNumber = BCDNumber-10;
	}
	
	BCDNumber = BCDNumber+10;
	UnitsCount = BCDNumber;
	
}

//******************************************************************************************************************
//��������� ���������� � ��������� ������� �����������
uint8_t ReadScratchPadFromDS18B20( void )
{
	uint8_t ReadStatus, i;
	
	Reset1WireLine();									//����� ��������� �� ����� � ���� ������� ����������
	WriteByteToDS18B20(SKIP_ROM);							//���������� ���������
	WriteByteToDS18B20(READ_SCRATCHPAD);				//�������� ������ ������, �������� 9 ����, 4� ���� ���������
	for (i=0; i<9; i++)
	{
		ScratchPadBuff[i] = ReadByteFrom1Wire();		//���������� ������, 9 ����
	}
	//������ ������ �������� CRC
	ReadStatus = ScratchPadBuff[4];
	return(ReadStatus);					//
}



//******************************************************************************************************************
//��������� ��������� ������� �����������
void SetConfigRegisterDS18B20(void)
{
	uint8_t ConfigRegister = 0;
	
	ConfigRegister &= ~((1<<R1)|(1<<R0));			//������������ ��� �������������� 9 ���
	Reset1WireLine();									//����� ��������� �� ����� � ���� ������� ����������
	WriteByteToDS18B20(SKIP_ROM);					//���������� ���������
	WriteByteToDS18B20(WRITE_SCRATCHPAD);				// ������ ������, �������� 9 ����, 4� ���� ���������
	WriteByteToDS18B20(TemperatureHigh);			//������� ����� ������������ ������ ������� ����������� �� �������� �������
	WriteByteToDS18B20(TemperatureLow);					//������ ����� ������������ ������ ������� ����������� �� �������� �������
	WriteByteToDS18B20(ConfigRegister);				//��������� ����������� ��������������
}


//******************************************************************************************************************
//�������� ������� � 1-Wire ����������
//void SendCommandDS18B20(uint8_t Command)
//{
	//volatile uint8_t i,SendBit;
		//
	//SendBit = (~(0xFE) & Command);				//��������� �������� ����		
	////Reset1WireLine();								//����� ��������� �� ����� � ���� ������� ����������
	//for (i=0; i<8; i++)							//���� �������� 8-�� ���
		//{
			//
			//if (SendBit==1)
				//{
					//WriteBit1To1Wire();			//������ ��� 1
				//}
			//else
				//{
					//WriteBit0To1Wire();		//������ ��� 0
				//}
			//Command=Command>>1;						//���������� �������� ����
			//SendBit = (~(0xFE) & Command);		//��������� �������� ����
			//Delay_nop();							//����� ����� ����-����� �������� ������ ���� (����������� ����� ����� ����-������� ������� 1���)
			//
		//}
			//
//}



//******************************************************************************************************************
//�������� ����� � 1-Wire ����������
void WriteByteToDS18B20(uint8_t Byte)
{
	volatile uint8_t i, SendBit;
	
	SendBit = (~(0xFE) & Byte);					//��������� �������� ����	
	for (i=0; i<8; i++)						//���� �������� 8-�� ���
	{
			
		if (SendBit==1)
			{
				WriteBit1To1Wire();				//������ ��� 1
			}
		else
			{
				WriteBit0To1Wire();			//������ ��� 0
			}
		Byte=Byte>>1;							//���������� �������� ����
		SendBit = (~(0xFE) & Byte);			//��������� �������� ����
		Delay_nop();							//����� ����� ����-����� �������� ������ ���� (����������� ����� ����� ����-������� ������� 1���)
			
	}
	
}

//******************************************************************************************************************
//���������� ����� �� 1-Wire ����������
uint8_t ReadByteFrom1Wire(void)
{
	uint8_t Temp_0;
	volatile uint8_t i, Byte;
	
	Byte = 0;								//��������� ����������, ����������� ���� � ���� 1-Wire
	
	for (i=0; i<8; i++)							//���� ����� ������� ��� (����� 8)
		{
			Byte = Byte>>1;					//����� ������ �� 1 ���, ���� ����������� ������� ����� �����, 1 ����� ������ � ������ �����
			Temp_0 = ReadBitFrom1Wire();		//������� ��� � ���� 1-Wire		
			if (Temp_0 == 1)				//���� ��� "1"
				{
					Byte |=0x80;				//�������� �� ����� �������� ���� 1
				} 
			else  
				{
					Byte |=0x00;			//�������� �� ����� �������� ���� 0 (��� ����� ������� ����� ���� �������)
				}
			Delay_nop();						//����� ����� ����-����� �������� ������ ���� (����������� ����� ����� ����-������� ������� 1���)

		}	
		
	return(Byte);							//�������� �������� ����������� �����
	
}



//******************************************************************************************************************
//����� ���� 1-Wire � ���� ��������� ���������� ���������
uint8_t Reset1WireLine(void)
{
	volatile uint8_t Error;
	
	asm("cli");																//���������� ���������� ����������			
	DS1821_DIRECT_PORT |= (1<<DS1821_DIRECT_PIN);								//	
	DS1821_DATA_PORT |= (0<<DS1821_DATA_PIN_OUT);							//�������� ����� � 0
	_delay_us(480);//480
	DS1821_DIRECT_PORT &= ~(1<<DS1821_DIRECT_PIN);								//��������� �����
	_delay_us(70);															//�������� �� 60 �� 240��� 70, ����� 120���
	Error = ((DS1821_DATA_PIN_IN & (~(READ_BIT_FROM_PORT)))>>ShiftingBit);		//�������� 3� ��� � �������� �� ������� �������� ����	
	_delay_us(510);															//&&&
	asm("sei");																	//���������� ���������� ����������
	return(Error);															//�������� ���� ������ (��������� ����� � ������ �������� ������� �� ����������)
}

//******************************************************************************************************************
//������ ���� � ���� 1-Wire 
void WriteBit0To1Wire(void)
{
	asm("cli");//���������� ���������� ����������
		
	DS1821_DIRECT_PORT |= (1<<DS1821_DIRECT_PIN);					//
	DS1821_DATA_PORT |= (0<<DS1821_DATA_PIN_OUT);						//�������� ����� � 0
	_delay_us(60);	
	DS1821_DIRECT_PORT &= ~(1<<DS1821_DIRECT_PIN);					//��������� �����
	_delay_us(10);														//�����, ����������� ��� �������������� ����� � �������� ��������� ���. 1		
	asm("sei");//���������� ���������� ����������
}

//******************************************************************************************************************
//������ ������� � ���� 1-Wire
void WriteBit1To1Wire(void)
{
	asm("cli");//���������� ���������� ����������
		
	DS1821_DIRECT_PORT |= (1<<DS1821_DIRECT_PIN);					//
	DS1821_DATA_PORT |= (0<<DS1821_DATA_PIN_OUT);						//�������� ����� � 0
	_delay_us(5);	
	DS1821_DIRECT_PORT &= ~(1<<DS1821_DIRECT_PIN);					//��������� �����
	_delay_us(64);	
	asm("sei");//���������� ���������� ����������
}

//******************************************************************************************************************  
//������ ���� � ���� 1-Wire
uint8_t ReadBitFrom1Wire(void)
{
	uint8_t ReadBit;
	
	asm("cli");																//���������� ���������� ����������	
	DS1821_DATA_PORT |= (0<<DS1821_DATA_PIN_OUT);								//�������� ����� � 0
	DS1821_DIRECT_PORT |= (1<<DS1821_DIRECT_PIN);							//
	_delay_us(5);//����� 6 ���
	DS1821_DIRECT_PORT &= ~(1<<DS1821_DIRECT_PIN);								//��������� �����
	_delay_us(9);//����� 9 ���
	ReadBit = ((DS1821_DATA_PIN_IN & (~(READ_BIT_FROM_PORT)))>>ShiftingBit);//�������� 3� ��� � �������� �� ������� �������� ����
	_delay_us(55);	
	asm("sei");																	//���������� ���������� ����������
	return(ReadBit);
}

//******************************************************************************************************************
//�������� 2��� (������� �� 4���)
void Delay_nop(void)
{
	asm("nop");asm("nop");asm("nop");asm("nop");
	asm("nop");asm("nop");asm("nop");asm("nop");
	//asm("nop");asm("nop");asm("nop");asm("nop");
	//asm("nop");asm("nop");asm("nop");asm("nop");
}