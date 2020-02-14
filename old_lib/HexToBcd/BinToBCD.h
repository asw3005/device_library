/* Created: 17.06.2018
 * BinToBCD.h
 * 
 * 
 * 
 */	
 
#ifndef BinToBCD_H_
#define BinToBCD_H_

#include "stm32f10x.h"

//***************************************************************************************************************************************
//Константы
#define    Sign                  0x00
#define    HundredThousands      0x01
#define    ThenThousands         0x02
#define    Thousands             0x03
#define    Hundreds              0x04
#define    Tens                  0x05
#define    Units                 0x06

//***************************************************************************************************************************************
//Прототипы функций
void BinToBcd(int32_t InNumder);//Преобразование числа из двоичного в двоично-десятичный формат
void TmpConvToTx(int32_t Tmp);          //Подготовка для передачи температуры
void PressConvToTx(uint32_t Press);      //Подготовка для передачи давления
void HumConvToTx(uint32_t Hum);          //Подготовка для передачи влажности
  
#endif /* BinToBCD_H_ */


