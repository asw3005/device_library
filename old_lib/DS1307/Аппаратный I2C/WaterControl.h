/*
 * WaterControl.h
 *
 * Created: 17.11.2014 22:40:13
 *
 */ 


#ifndef WATERCONTROL_H_
#define WATERCONTROL_H_

//************************************************************************************************************************************
//���������� �������� ������������

#define HeatingElementEnable    PORTD |= (1<<PORTD0)                    //�������� �������������� �������
#define HeatingElementDisable   PORTD &= (~(1<<PORTD0))                 //��������� �������������� �������
#define PumpEnable              PORTD |= (1<<PORTD1)                    //�������� �����
#define PumpDisable             PORTD &= (~(1<<PORTD1))                 //��������� �����

//************************************************************************************************************************************
//��������� ������ ������� ���������

#define LedHeatingEnable		PORTD |= (1<<PORTD2)					//�������� ��������� ��������� �������
#define LedHeatingDisable		PORTD &= (~(1<<PORTD2))					//�������� ��������� ���������� �������
#define LedPumpEnable			PORTD |= (1<<PORTD3)					//�������� ��������� ��������� ������
#define LedPumpDisable			PORTD &= (~(1<<PORTD3))					//�������� ��������� ���������� ������

//************************************************************************************************************************************
//��������� ������� ������

#define LockRefillWater			PORTD |= (1<<PORTD4)					//������������ ���������� ���������� ���� � ������� ����� ��������
#define UnLockRefillWater		PORTD &= (~(1<<PORTD4))					//������������ ���������� ���������� ���� � ������� ����� ���������


#endif /* WATERCONTROL_H_ */