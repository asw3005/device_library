/*
 * DAC_AD5432.h
 *
 * Created: 03.07.2014 12:54:44
 *  Author: 554-pea
 */ 
#include <avr/io.h>

#ifndef DAC_AD5432_H_
#define DAC_AD5432_H_


//************************************************************************************************************
//���������� ������� ����, ���� 15 - 12, C3 C2 C1 C0

#define NOP_CMD							0b00000000		//No operation (power-on default)
#define LOAD_AND_UPDATE					0b00010001			//Load and update
#define INITIATE_READBACK				0b00100010		//Initiate readback
#define DAISY_CHAIN_DISABLE				0b10011001			//Daisy-chain disable
#define CLK_TO_SHIFTREG_ONRISING_EDGE	0b10101010		//Clock data to shift register on rising edge
#define DAC_OUT_TO_ZERO_SCALE			0b10111011			//Clear DAC output to zero scale
#define DAC_OUT_TO_MIDSCALE				0b11001100		//Clear DAC output to midscale

//************************************************************************************************************
//������������ ����� �����-������ � ������ ��

#define DAC_PORT	PORTB

#define DAC_SYNC	PB5			//����� ������������� ���
#define DAC_CLK		PB6		//����� �������� ��������� ���
#define DAC_DATA	PB7			//����� ������ ���

//************************************************************************************************************
//��������� ������� ��� ������� � �����

void DacInit(void);										//������������� ����
void SendDataToDac(uint8_t Command, uint16_t Data);	//������������� ����
void UpDown(void);										//���� ���� �������� ��������� ������ ����� ����� � ������ ����
void SetToMidScale(void);							//���������� ��� �� �������� ���������
void SetToZeroScale(void);								//���������� ��� � 0
void SetToMaxScale(void);							//���������� ��� �� ��������

#endif /* DAC_AD5432_H_ */