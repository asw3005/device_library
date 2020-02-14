/* Created: 03.01.2015 18:27:34
 * I2C.h
 *
 * 
 *
 */ 


#ifndef I2C_H_
#define I2C_H_

#include <avr/io.h>

extern uint8_t DirectionData;
extern uint8_t OffsetAddrRW;
extern uint8_t NumberByteRW;
extern uint8_t CurrentAddressRW;
extern uint8_t RWDeviceFlag;
extern uint8_t *RWBytePointer;

//*****************************************************************************************
//Общие коды состояния I2C
#define START_CONDITION_I2C1			0x08//A START condition has been transmitted
#define REPEATED_START_I2C1				0x10//A repeated START condition has been transmitted
#define ARBITRATION_LOST_I2C1			0x38//Arbitration lost in SLA+W or data bytes 

//*****************************************************************************************
//Коды состояния Мастера передатчика
#define SLADDR_W_RECEIVED_ACK			0x18// SLA+W has been transmitted
#define SLADDR_W_RECEIVED_NACK			0x20// SLA+W has been transmitted
#define DATA_TRANSMITTED_ACK_RECEIVED	0x28// Data byte has been transmitted
#define DATA_TRANSMITTED_NACK_RECEIVED	0x30// Data byte has been transmitted


//*****************************************************************************************
//Коды состояния Мастера приёмника
#define SLADDR_R_RECEIVED_ACK			0x40//SLA+R has been transmitted, ACK has been received
#define SLADDR_R_RECEIVED_NACK			0x48//SLA+R has been transmitted, NOT ACK has been received
#define DATA_RECEIVED_ACK_RETURNED		0x50//Data byte has been received, ACK has been returned
#define DATA_RECEIVED_NACK_RETURNED		0x58//Data byte has been received, NOT ACK has been returned
				
//*****************************************************************************************
//Регистр управления автоматом I2C

#define SEND_START						(TWCR |= ((1<<TWINT)|(1<<TWSTA)|(1<<TWEN)|(1<<TWIE)))
#define SEND_STOP						(TWCR |= ((1<<TWINT)|(1<<TWSTO)|(1<<TWEN)|(1<<TWIE)))
#define SEND_BYTE						(TWCR |= ((1<<TWINT)|(1<<TWEN)|(1<<TWIE)))
#define READ_BYTE						(TWCR |= ((1<<TWINT)|(1<<TWEN)|(1<<TWIE)))
#define READ_BYTE_ACK					(TWCR |= ((1<<TWINT)|(1<<TWEN)|(1<<TWIE)|(1<<TWEA)))

//*****************************************************************************************
//Маски
#define TWSR_MASK						0x07
#define CLEAR_START_BIT_MASK			0x20
#define TWSR_PRESCALER_VALUE			0x00// 00-1;01-4;10-16;11-64;
#define TWBR_BIT_VALUE					0x05// Максимум 8 бит значение



//*****************************************************************************************
//Порт I2C интерфейса


				


void I2C_Init(void);		//Настройка автомата I2C (частоты передачи данных SCL)
void SendByteI2C(void);			//Отправить определённое количество байт, по окончанию сгенерировать СТОП
uint8_t ReadByteI2C(void);	//Принять байт

#endif /* I2C_H_ */