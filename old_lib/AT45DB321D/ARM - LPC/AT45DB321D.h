#include "LPC17xx.h"
#include "main.h"

extern uint8_t RWFlashBuffer528[528];
extern uint8_t RWFlashBuffer512[512];

//*************************************************************************************************
//Read Commands
#define	MAIN_MEMORY_PAGE_READ 								0xD2	//Group A
#define	CONTINUOUS_ARRAY_READ_LEGACY_COMMAND 				0xE8	//
#define	CONTINUOUS_ARRAY_READ_LOW_FREQUENCY 				0x03	//
#define	CONTINUOUS_ARRAY_READ_HIGH_FREQUENCY 				0x0B	//
#define	BUFFER1_READ_LOW_FREQUENCY 							0xD1	//
#define	BUFFER2_READ_LOW_FREQUENCY 						    0xD3	//
#define	BUFFER1_READ 										0xD4	//Group C
#define	BUFFER2_READ										0xD6	//Group C

//*************************************************************************************************
//Program and Erase Commands
#define	BUFFER1_WRITE 										0x84	//Group C
#define	BUFFER2_WRITE 										0x87	//Group C
#define	BUFFER1_TO_MAIN_MEMORY_PAGE_WITH_BUILT_IN_ERASE 	0x83	//Group B
#define	BUFFER2_TO_MAIN_MEMORY_PAGE_WITH_BUILT_IN_ERASE 	0x86	//Group B
#define	BUFFER1_TO_MAIN_MEMORY_PAGE_WITHOUT_BUILT_IN_ERASE 	0x88	//Group B
#define	BUFFER2_TO_MAIN_MEMORY_PAGE_WITHOUT_BUILT_IN_ERASE 	0x89	//Group B
#define	PAGE_ERASE 											0x81	//Group B
#define	BLOCK_ERASE 										0x50	//Group B
#define	SECTOR_ERASE 										0x7C	//Group B
//****************************************************
//CHIP_ERASE CHAIN COMMANDS
#define	CHIP_ERASE0 										0xC7	//Group B
#define	CHIP_ERASE1 										0x94	//
#define	CHIP_ERASE2 										0x80	//
#define	CHIP_ERASE3										    0x9A	//
//****************************************************
#define	MAIN_MEMORY_PAGE_THROUGH_BUFFER1    				0x82	//Group B
#define	MAIN_MEMORY_PAGE_THROUGH_BUFFER2    		        0x85	//Group B
//****************************************************
//ENABLE_SECTOR_PROTECTION CHAIN COMMANDS
#define	ENABLE_SECTOR_PROTECTION0   		 				0x3D	//
#define	ENABLE_SECTOR_PROTECTION1 			 				0x2A	// 
#define	ENABLE_SECTOR_PROTECTION2 			 				0x7F	//
#define	ENABLE_SECTOR_PROTECTION3 			 				0xA9	//
//****************************************************
//DISABLE_SECTOR_PROTECTION CHAIN COMMANDS
#define	DISABLE_SECTOR_PROTECTION0 							0x3D	//
#define	DISABLE_SECTOR_PROTECTION1 							0x2A	//
#define	DISABLE_SECTOR_PROTECTION2 							0x7F	//
#define	DISABLE_SECTOR_PROTECTION3 							0x9A	//
//****************************************************
//ERASE_SECTOR_PROTECTION_REGISTER CHAIN COMMANDS
#define	ERASE_SECTOR_PROTECTION_REGISTER0 					0x3D	//Group D
#define	ERASE_SECTOR_PROTECTION_REGISTER1 					0x2A	//
#define	ERASE_SECTOR_PROTECTION_REGISTER2 					0x7F	//
#define	ERASE_SECTOR_PROTECTION_REGISTER3 				    0xCF	//
//****************************************************
//PROGRAM_SECTOR_PROTECTION_REGISTER CHAIN COMMANDS
#define	PROGRAM_SECTOR_PROTECTION_REGISTER0 			    0x3D	//Group D
#define	PROGRAM_SECTOR_PROTECTION_REGISTER1 				0x2A	//
#define	PROGRAM_SECTOR_PROTECTION_REGISTER2 				0x7F	//
#define	PROGRAM_SECTOR_PROTECTION_REGISTER3 				0xFC	//
//****************************************************
#define	READ_SECTOR_PROTECTION_REGISTER			 			0x32	//Group A
//SECTOR_LOCKDOWN CHAIN COMMANDS
#define	SECTOR_LOCKDOWN0 									0x3D	//Group D 
#define	SECTOR_LOCKDOWN1 									0x2A	//
#define	SECTOR_LOCKDOWN2 									0x7F	//
#define	SECTOR_LOCKDOWN3 									0x30	//
//****************************************************
#define	READ_SECTOR_LOCKDOWN_REGISTER						0x35	//Group A
//****************************************************
//PROGRAM_SECURITY_REGISTER CHAIN COMMANDS
#define	PROGRAM_SECURITY_REGISTER0 							0x9B	//Group D 
#define	PROGRAM_SECURITY_REGISTER1 							0x00	//
#define	PROGRAM_SECURITY_REGISTER2 							0x00	//
#define	PROGRAM_SECURITY_REGISTER3 							0x00	//
//****************************************************
#define	READ_SECURITY_REGISTER 								0x77	//Group A

//*************************************************************************************************
//Additional Commands
#define	MAIN_MEMORY_PAGE_TO_BUFFER1_TRANSFER 				0x53	//Group B
#define	MAIN_MEMORY_PAGE_TO_BUFFER2_TRANSFER 				0x55	//Group B
#define	MAIN_MEMORY_PAGE_TO_BUFFER1_COMPARE 				0x60	//Group B
#define	MAIN_MEMORY_PAGE_TO_BUFFER2_COMPARE 				0x61	//Group B
#define	AUTO_PAGE_REWRITE_THROUGH_BUFFER1 					0x58	//Group B
#define	AUTO_PAGE_REWRITE_THROUGH_BUFFER2 					0x59	//Group B
#define	DEEP_POWER_DOWN 									0xB9	//
#define	RESUME_FROM_DEEP_POWER_DOWN 						0xAB	//
#define	STATUS_REGISTER_READ 								0xD7	//Group C
#define	MANUFACTURER_AND_DEVICE_ID_READ 					0x9F	//Group C

//*************************************************************************************************
//Legacy Commands
#define	BUFFER1_LEGACY_READ									0x54	//
#define	BUFFER2_LEGACY_READ									0x56	//
#define	MAIN_MEMORY_PAGE_LEGACY_READ						0x52	//
#define	CONTINUOUS_ARRAY_LEGACY_READ    					0x68	//Group A
#define	STATUS_REGISTER_LEGACY_READ							0x57	//

//*************************************************************************************************
#define 		STATUS_FLASH_RDY							0x00

//*************************************************************************************************
//Определения для используемых портов ввода-вывода

#define			PINSET			FIOSET
#define			PINRESET		FIOCLR

#define			SPI_PORT		LPC_GPIO0			

//#define			DATA_IN		PIN18			
//#define			RDY				PIN19										

#define			DATA_OUT		    PIN18									//константы для изменения уровня на выбранном выводе
#define			CLK					PIN15
#define			CS1					PIN16
#define			CS2					PIN22
#define			RESET				PIN20
#define			WP					PIN21



void ReadFlashPage528(uint8_t NumberFlash, uint8_t NumberPage);//Чтение страницы флешь 1 или 2 (для страницы 528 байт)
void WriteFlashPage528C(uint8_t NumberFlash, uint8_t NumberBuffer, uint8_t NumberPage);//Запись страницы флешь 1 или 2 с последующей записью (для страницы 528 байт)

void SendCommand(uint8_t Command);								                    //Передача команды
void ClearFlash528(void);											                    //Передача 24х бит для стирания флешь секторами (для страницы 528 байт)
void SendClearSectorAB528(uint8_t SectorAddr);		                                //Стирание сектора 0а и 0b (для страницы 528 байт)
void SendClearSector528(uint8_t SectorAddr);		                                    //Стирание сектора (1-63, для страницы 528 байт)
void ClearFlash512(void);												            //Передача 24х бит для стирания флешь секторами (для страницы 512 байт)
void SendClearSectorAB512(uint8_t SectorAddr);	                                        //Стирание сектора 0а и 0b (для страницы 512 байт)
void SendClearSector512(uint8_t SectorAddr);			                            //Стирание сектора (1-63, для страницы 512 байт)
void StatusRead(void);												                    //Проверка флага занятости
void OneClockEnable(void);												            //Сигнал тактирования
void StartAddrByteWrite528(uint16_t PageAddr, uint16_t StartByteAddr);                  //Установка начального адреса страницы и считываемого байта (для страницы 528 байт)
void StartAddrByteWrite512(uint16_t PageAddr, uint16_t StartByteAddr);              //Установка начального адреса страницы и считываемого байта (для страницы 512 байт)
uint8_t ByteRead528(uint16_t PageAddr, uint16_t NumberByte);                            //Чтение байта (для страницы 512 байт)
uint8_t ByteRead512(uint16_t PageAddr, uint16_t NumberByte);                        //Чтение байта (для страницы 512 байт)
void PageRead528(uint16_t PageAddr);                                                    //Чтение страницы (для страницы 512 байт)
void PageRead512(uint16_t PageAddr);                                                //Чтение страницы (для страницы 512 байт)
uint8_t ReadByteFromLine(void);                                                         //Чтение байта с SPI выхода флешь
void BufferWrite528(uint8_t NumberBuffer, uint16_t BufferAddr);   //Запись данных в буфер 1 или 2 (для страницы 528 байт), команда группы С
void BufferWrite512(uint8_t NumberBuffer, uint16_t BufferAdd);       //Запись данных в буфер 1 или 2 (для страницы 512 байт), команда группы С
void WritePage528 (uint8_t NumberBuffer, uint16_t PageAddr);                        //Запись данных из буфера 1 или 2 на произвольную страницу (для страницы 528 байт)
void WritePage512 (uint8_t NumberBuffer, uint16_t PageAddr);                            //Запись данных из буфера 1 или 2 на произвольную страницу (для страницы 512 байт)

void PageProgramThroughBuffer528 (uint8_t NumberBuffer,uint16_t PageAddr, uint16_t BufferAddr); //Запись данных из буфера 1 или 2 на произвольную страницу c предварительным заполнением буфера (командой группы Б) (для страницы 528 байт)
void PageProgramThroughBuffer512 (uint8_t NumberBuffer,uint16_t PageAddr, uint16_t BufferAddr);     //Запись данных из буфера 1 или 2 на произвольную страницу c предварительным заполнением буфера (командой группы Б) (для страницы 528 байт)
void SendBitToLine(uint8_t NumberBit, uint16_t Addr, uint16_t Mask, uint16_t Compare);          //Отправка бит по SPI - параметры: кол-во бит, адрес байта, маска, число сравнения





