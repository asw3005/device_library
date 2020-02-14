/* Created: 27.11.2014 12:36:09
 * DS3231.h
 *
 * 
 *
 */ 


#ifndef   DS3231_H_
#define   DS3231_H_

//***************************************************************************************************************************************
//Константы

#define   ADDR_DS3231                            0xEC                     //7 bit MSB 0x76 (address) + 1 bit LSB 0x00 (read/write)
#define   WRITE_MODE									           0x00											//режим записи в ведомое устройство				
#define   READ_MODE										           0x01											//режим чтения из ведомого устройства

#define   ADDR_DS3231_WRITE_MODE	        (ADDR_DS3231 | WRITE_MODE)		  //Отправить адрес ведомого устройства, режим запись 
#define   ADDR_DS3231_READ_MODE				    (ADDR_DS3231 | READ_MODE)			  //Отправить адрес ведомого устройства, режим чтение

#define   DS3231_SECONDS_ADDR                     0x00                    //Единицы секунд и десятки секунд
#define   DS3231_MINUTES_ADDR                     0x01                    //Единицы минут и десятки минут
#define   DS3231_HOURS_ADDR                       0x02                    //Единицы часов и десятки часов + формат 12/24
#define   DS3231_DAY_ADDR                         0x03                    //День недели
#define   DS3231_DATE_ADDR                        0x04                    //Число, единицы и десятки
#define   DS3231_MONTH_CENTURY_ADDR               0x05                    //Месяц, единицы и десятки
#define   DS3231_YEAR_ADDR                        0x06                    //Год, единицы и десятки
#define   DS3231_ALARM_1_SECONDS_ADDR             0x07                    //Будильник 1, секунды и десятки секунд
#define   DS3231_ALARM_1_MINUTES_ADDR             0x08                    //Будильник 1, минуты и десятки минут
#define   DS3231_ALARM_1_HOURS_ADDR               0x09                    //Будильник 1, часы и десятки часов +  формат 12/24
#define   DS3231_ALARM_1_DAY_DATE_ADDR            0x0A                    //Будильник 1, день недели или единицы числа + десятки числа, + режим срабатывания день (1) или дата (0)
#define   DS3231_ALARM_2_MINUTES_ADDR             0x0B                    //Будильник 2, минуты и десятки минут
#define   DS3231_ALARM_2_HOURS_ADDR               0x0C                    //Будильник 2, часы и десятки часов +  формат 12/24
#define   DS3231_ALARM_2_DAY_DATE_ADDR            0x0D                    //Будильник 2, день недели или единицы числа + десятки числа, + режим срабатывания день (1) или дата (0)
#define   DS3231_CONTROL_ADDR                     0x0E                    //Регистр управления
#define   DS3231_CONTROL_STATUS_ADDR              0x0F                    //Регистр управления и статуса
#define   DS3231_AGING_OFFSET_ADDR                0x10                    //
#define   DS3231_MSB_OF_TEMP_ADDR                 0x11                    // 
#define   DS3231_LSB_OF_TEMP_ADDR                 0x12                    // 

//***************************************************************************************************************************************
//Расположение данных в буфере передачи настройка DS3231
#define   AddressDS3231                           0x00
#define   AddressControl                          0x01
#define   DataControl                             0x02
#define   AddressControlStatus                    0x03
#define   DataControlStatus                       0x04
#define   AddressAging                            0x05
#define   DataAging                               0x06
      
//***************************************************************************************************************************************
//Расположение данных в буфере передачи настройка времени
#define   AddressDS3231                           0x00
#define   AddressSeconds                          0x01
#define   DataSeconds                             0x02
#define   AddressMinutes                          0x03
#define   DataMinutes                             0x04
#define   AddressHours                            0x05
#define   DataHours                               0x06
#define   AddressDay                              0x07
#define   DataDay                                 0x08
#define   AddressDate                             0x09
#define   DataDate                                0x0A
#define   AddressMonthCentury                     0x0B
#define   DataMonthCentury                        0x0C
#define   AddressYear                             0x0D
#define   DataYear                                0x0E

//***************************************************************************************************************************************
//Данные адреса расположения данных в буфере приёма
#define   SECONDS                                 0x00    
#define   MINUTES                                 0x01    
#define   HOURS                                   0x02    
#define   DAY                                     0x03    
#define   DATE                                    0x04    
#define   MONTH_CENTURY                           0x05    
#define   YEAR                                    0x06    
#define   ALARM_1_SECONDS                         0x07    
#define   ALARM_1_MINUTES                         0x08    
#define   ALARM_1_HOURS                           0x09    
#define   ALARM_1_DAY_DATE                        0x0A    
#define   ALARM_2_MINUTES                         0x0B    
#define   ALARM_2_HOURS                           0x0C    
#define   ALARM_2_DAY_DATE                        0x0D    
#define   CONTROL                                 0x0E    
#define   CONTROL_STATUS                          0x0F    
#define   AGING_OFFSET                            0x10    
#define   MSB_OF_TEMP                             0x11    
#define   LSB_OF_TEMP                             0x12    

//***************************************************************************************************************************************
//Прототипы функций
void DS3231PreparationConfig(void);                       //Подготовка конфигурационных данных (работа внутри помещения)
void DS3231WriteConfig(void);                             //Настройка DS3231
void DS3231WriteTime(void);                               //Настройка времени
void getDS3231Data0(void);                                //Получить данные DS3231
void getDS3231DataTemp(void);                             //Получить данные о температуре 
void DS3231WriteAddress(uint8_t RxTxCondition);           //Отправка адреса устройства


#endif /* DS3231_H_ */

