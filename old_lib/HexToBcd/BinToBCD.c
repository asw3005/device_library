/* Created: 17.06.2018
 * BinToBCD.h
 * 
 * 
 * 
 */	
 
#include "stm32f10x.h"
#include "BinToBCD.h"
#include "USART2.h"

uint8_t BCDBuffer[7];
extern uint8_t TxBufferUart2[14];

//************************************************************************************************
//Подготовка для передачи температуры
void TmpConvToTx(int32_t Tmp)
{
  int8_t ConvertionValue;
  
  BinToBcd(Tmp);
  ConvertionValue = BCDBuffer[Thousands]*10;
  ConvertionValue = ConvertionValue + BCDBuffer[Hundreds];
  if (BCDBuffer[Sign] == 1)
  {TxBufferUart2[Tmsb] = ~ConvertionValue + 1;}
  else{TxBufferUart2[Tmsb] = ConvertionValue;}
  
  ConvertionValue = BCDBuffer[Tens]*10;
  ConvertionValue = ConvertionValue + BCDBuffer[Units];
  TxBufferUart2[Tlsb] = ConvertionValue;  
}

//************************************************************************************************
//Подготовка для передачи давления
void PressConvToTx(uint32_t Press)
{
  uint16_t ConvertionValue;

  BinToBcd(Press);
  ConvertionValue = BCDBuffer[HundredThousands]*100;
  ConvertionValue = ConvertionValue + BCDBuffer[ThenThousands]*10;
  ConvertionValue = ConvertionValue + BCDBuffer[Thousands]; 
  TxBufferUart2[Pmsb] = ConvertionValue>>8;
  TxBufferUart2[Pxmsb] = ConvertionValue & 0x00FF;
  
  ConvertionValue = BCDBuffer[Hundreds]*100;
  ConvertionValue = ConvertionValue + BCDBuffer[Tens]*10;
  ConvertionValue = ConvertionValue + BCDBuffer[Units];
  TxBufferUart2[Plsb] = ConvertionValue>>8;
  TxBufferUart2[Pxlsb] = ConvertionValue & 0x00FF;
}

//************************************************************************************************
//Подготовка для передачи влажности
void HumConvToTx(uint32_t Hum)
{
  uint8_t ConvertionValue;
  
  BinToBcd(Hum);
  ConvertionValue = BCDBuffer[ThenThousands]*10;
  ConvertionValue = ConvertionValue + BCDBuffer[Thousands];  
  TxBufferUart2[RHmsb] = ConvertionValue;
  
  ConvertionValue = BCDBuffer[Hundreds]*10;
  ConvertionValue = ConvertionValue + BCDBuffer[Tens];
  TxBufferUart2[RHlsb] = ConvertionValue;
}

//************************************************************************************************
//Преобразование числа из двоичного в двоично-десятичный формат
void BinToBcd(int32_t InNumder)
{
  uint8_t Counter = 0;
  
  if (InNumder < 0)
    {
      InNumder = ~InNumder + 1;
      BCDBuffer[Sign] = 1;
    }
  else {BCDBuffer[Sign] = 0;}
  
  while(InNumder >= 100000)
    {
      InNumder = InNumder - 100000;
      Counter++;
    }
  BCDBuffer[HundredThousands] = Counter;
  Counter = 0; 
    
  while(InNumder >= 10000)
    {
      InNumder = InNumder - 10000;
      Counter++;
    }
  BCDBuffer[ThenThousands] = Counter;
  Counter = 0; 
    
  while(InNumder >= 1000)
    {
      InNumder = InNumder - 1000;
      Counter++;
    }
  BCDBuffer[Thousands] = Counter;
  Counter = 0; 
    
  while(InNumder >= 100)
    {
      InNumder = InNumder - 100;
      Counter++;
    }
  BCDBuffer[Hundreds] = Counter;
  Counter = 0;
    
  while(InNumder >= 10)
    {
      InNumder = InNumder - 10;
      Counter++;
    }
  BCDBuffer[Tens] = Counter;
  Counter = 0; 
    
  while(InNumder > 0)
    {
      InNumder = InNumder - 1;
      Counter++;
    }   
  BCDBuffer[Units] = Counter;
  Counter = 0;   
}
			
