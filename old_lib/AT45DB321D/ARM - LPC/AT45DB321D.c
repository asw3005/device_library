#include "LPC17xx.h"
#include "AT45DB321D.h"
#include "main.h"

uint8_t RWFlashBuffer528[528] ;//__attribute__((section ("RW")))

uint8_t RWFlashBuffer512[512] ;//__attribute__((section ("RW")))

//*************************************************************************************************
// (для страницы 528 байт)
void ClearFlash528(void)
{
	volatile uint8_t i;
	
//	for (i=0; i<128; i++)					//стирание секторов 1-63
//			{
//				Chip1Enable();
//				SendClearSectorAB528(i);		//Стирание сектора 0а и 0b
//				Chip1Disable();					//начало стирания выбранной страницы
//				StatusRead();						//проверка занятости флешь
//			}
//			
//	for (i=1; i<64; i++)					//стирание секторов 1-63
//			{
//				Chip1Enable();
//				__NOP();
//				SendClearSector528(i);
//				Chip1Disable();					//начало стирания выбранной страницы
//				StatusRead();						//проверка занятости флешь
//			}
			
	for (i=0; i<128; i++)					//стирание секторов 1-63
			{
				Chip2Enable();
				SendClearSectorAB528(i);		//Стирание сектора 0а и 0b
				Chip2Disable();					//начало стирания выбранной страницы
				StatusRead();						//проверка занятости флешь
			}
			
	for (i=1; i<64; i++)					//стирание секторов 1-63
			{
				Chip2Enable();
				__NOP();
				SendClearSector528(i);
				Chip2Disable();					//начало стирания выбранной страницы
				StatusRead();						//проверка занятости флешь
			}

}

//*************************************************************************************************
//Передача 24х бит для стирания флешь секторами (для страницы 512 байт)
void ClearFlash512(void)
{
	volatile uint8_t i;
	
		for (i=0; i<128; i++)					//стирание секторов 1-63
			{
				Chip1Enable();
				SendClearSectorAB512(i);						//Стирание сектора 0а и 0b
				Chip1Disable();					//начало стирания выбранной страницы
				StatusRead();						//проверка занятости флешь
			}
			
	for (i=1; i<64; i++)					//стирание секторов 1-63
			{
				Chip1Enable();
				__NOP();
				SendClearSector512(i);
				Chip1Disable();					//начало стирания выбранной страницы
				StatusRead();						//проверка занятости флешь
			}
			
	for (i=0; i<128; i++)					//стирание секторов 1-63
			{
				Chip2Enable();
				SendClearSectorAB512(i);						//Стирание сектора 0а и 0b
				Chip2Disable();					//начало стирания выбранной страницы
				StatusRead();						//проверка занятости флешь
			}
			
	for (i=1; i<64; i++)					//стирание секторов 1-63
			{
				Chip2Enable();
				__NOP();
				SendClearSector512(i);
				Chip2Disable();					//начало стирания выбранной страницы
				StatusRead();						//проверка занятости флешь
			}

}

//*************************************************************************************************
//Чтение страницы флешь 1 или 2 (для страницы 528 байт)
void ReadFlashPage528(uint8_t NumberFlash, uint8_t NumberPage)
{
    if (NumberFlash == 1)
        {
            Chip1Enable();  
        }
    else
        {
            Chip2Enable();   
        }

    PageRead528(NumberPage);//чтение страницы

    Chip1Disable();
    Chip2Disable();
    StatusRead();   		//проверка занятости флешь
}

//*************************************************************************************************
//Запись страницы флешь 1 или 2 с последующей записью (для страницы 528 байт)
void WriteFlashPage528C(uint8_t NumberFlash, uint8_t NumberBuffer, uint8_t NumberPage)
{
    
    if (NumberFlash == 1)
        {
            Chip1Enable();  
        }
    else
        {
            Chip2Enable();   
        }

    BufferWrite528(NumberBuffer,0);//номер буфера 1, запись с начала

    Chip1Disable();
    Chip2Disable();
    StatusRead();						//проверка занятости флешь
        
    if (NumberFlash == 1)
        {
            Chip1Enable();  
        }
    else
        {
            Chip2Enable();   
        }
        
    WritePage528(NumberBuffer,NumberPage);//номер буфера 1, страница 1
    
    Chip1Disable();
    Chip2Disable();
    StatusRead();						//проверка занятости флешь

}


//*************************************************************************************************
//Чтение байта (для страницы 512 байт)
uint8_t ByteRead528(uint16_t PageAddr, uint16_t NumberByte)
{
	StartAddrByteWrite528(PageAddr, NumberByte);//установка страницы и читаемого байта
    return(ReadByteFromLine());
}

//*************************************************************************************************
//Чтение байта (для страницы 512 байт)
uint8_t ByteRead512(uint16_t PageAddr, uint16_t NumberByte)
{
    StartAddrByteWrite512(PageAddr, NumberByte);//установка страницы и читаемого байта
    return(ReadByteFromLine());
}

//*************************************************************************************************
//Чтение страницы (для страницы 512 байт)
void PageRead528(uint16_t PageAddr)
{
    volatile uint16_t i;
	
	StartAddrByteWrite528(PageAddr, 0);//установка страницы и читаемого байта
	for (i=0; i<528;i++)
        {
            RWFlashBuffer528[i] = ReadByteFromLine();
        }

}

//*************************************************************************************************
//Чтение страницы (для страницы 512 байт)
void PageRead512(uint16_t PageAddr)
{
    volatile uint16_t i;

    StartAddrByteWrite512(PageAddr, 0);//установка страницы и читаемого байта
    for (i=0; i<512;i++)
        {
            RWFlashBuffer512[i] = ReadByteFromLine();
        }
}

//*************************************************************************************************
//Запись данных в буфер 1 или 2 (для страницы 528 байт), команда группы С
void BufferWrite528(uint8_t NumberBuffer, uint16_t BufferAddr)
{
    uint16_t i;
    
    if (NumberBuffer == 1)
        {
            SendCommand(BUFFER1_WRITE);             //выбрать для записи буфер 1
        }
    else
        {
            SendCommand(BUFFER2_WRITE);                 //выбрать для записи буфер 2
        }
   SPI_PORT->PINRESET = DATA_OUT;
   for (i=0; i<14; i++)							    //передача 14-ти незначащих бит 
        {
            OneClockEnable();					        //защёлкнуть 1 бит в МС памяти
        }
   
   SendBitToLine(10, BufferAddr, 0xFDFF, 0x0200);   //передача 10 бит адреса буффера
        
   for (i=0; i<528; i++)
        {
            SendBitToLine(8, RWFlashBuffer528[i], 0xFF7F, 0x0080);   //передача данных в буфер     
        }
           
}

//*************************************************************************************************
//Запись данных в буфер 1 или 2 (для страницы 512 байт), команда группы С
void BufferWrite512(uint8_t NumberBuffer, uint16_t BufferAddr)
{
    uint16_t i;
    
    if (NumberBuffer == 1)
        {
            SendCommand(BUFFER1_WRITE);             //выбрать для записи буфер 1
        }
    else
        {
            SendCommand(BUFFER2_WRITE);                 //выбрать для записи буфер 2
        }
   SPI_PORT->PINRESET = DATA_OUT;
   for (i=0; i<15; i++)							    //передача 15-ти незначащих бит 
        {
            OneClockEnable();					        //защёлкнуть 1 бит в МС памяти
        }
   
   SendBitToLine(9, BufferAddr, 0xFEFF, 0x0100);   //передача 9 бит адреса буффера
      
   for (i=0; i<512; i++)
        {
            SendBitToLine(8, RWFlashBuffer512[i], 0xFF7F, 0x0080);   //передача данных в буфер     
        }
        
}

//*************************************************************************************************
//Запись данных из буфера 1 или 2 на произвольную страницу (для страницы 528 байт)
void WritePage528(uint8_t NumberBuffer, uint16_t PageAddr)
{
    uint16_t i;
    
    if (NumberBuffer == 1)
        {
            SendCommand(BUFFER1_TO_MAIN_MEMORY_PAGE_WITH_BUILT_IN_ERASE);//выбрать для записи буфер 1
        }
    else
        {
            SendCommand(BUFFER2_TO_MAIN_MEMORY_PAGE_WITH_BUILT_IN_ERASE);   //выбрать для записи буфер 2
        }

    SPI_PORT->PINRESET = DATA_OUT;
    OneClockEnable();
        
    SendBitToLine(13, PageAddr, 0xEFFF, 0x1000);                        //передача 13 бит адреса страницы
    
    SPI_PORT->PINRESET = DATA_OUT;
    for (i=0; i<10; i++)						                            //передача 10-ти незначащих бит 
        {
            OneClockEnable();					                        //защёлкнуть 1 бит в МС памяти
        }

}

//*************************************************************************************************
//Запись данных из буфера 1 или 2 на произвольную страницу (для страницы 512 байт)
void WritePage512 (uint8_t NumberBuffer, uint16_t PageAddr)
{
    uint16_t i;
    
    if (NumberBuffer == 1)
        {
            SendCommand(BUFFER1_TO_MAIN_MEMORY_PAGE_WITH_BUILT_IN_ERASE);//выбрать для записи буфер 1
        }
    else
        {
            SendCommand(BUFFER2_TO_MAIN_MEMORY_PAGE_WITH_BUILT_IN_ERASE);   //выбрать для записи буфер 2
        }

    SPI_PORT->PINRESET = DATA_OUT;
    OneClockEnable();
    OneClockEnable();
        
    SendBitToLine(13, PageAddr, 0xEFFF, 0x1000);                        //передача 13 бит адреса страницы
    
    SPI_PORT->PINRESET = DATA_OUT;
    for (i=0; i<9; i++)						                                //передача 9-ти незначащих бит 
        {
            OneClockEnable();					                        //защёлкнуть 1 бит в МС памяти
        }

}

//*************************************************************************************************
//Запись данных из буфера 1 или 2 на произвольную страницу c предварительным заполнением буфера (командой группы Б) (для страницы 528 байт)
void PageProgramThroughBuffer528 (uint8_t NumberBuffer,uint16_t PageAddr, uint16_t BufferAddr)
{

    uint16_t i;
    
    if (NumberBuffer == 1)
        {
            SendCommand(MAIN_MEMORY_PAGE_THROUGH_BUFFER1);          //выбрать для записи буфер 1
        }
    else
        {
            SendCommand(MAIN_MEMORY_PAGE_THROUGH_BUFFER2);              //выбрать для записи буфер 2
        }
    SPI_PORT->PINRESET = DATA_OUT;                                  //передача одного незначащего бита
    OneClockEnable();
        
    SendBitToLine(13, PageAddr, 0xEFFF, 0x1000);                        //передача 13 бит адреса страницы   
    SendBitToLine(10, BufferAddr, 0xFDFF, 0x0200);                  //передача 10 бит начального адреса в буфере
     
    for(i=0; i<528;i++)
        {
            SendBitToLine(8, RWFlashBuffer528[i], 0xFF7F, 0x0080);      //передача 528-ми байт в буффер, по 8 бит
        }

}

//*************************************************************************************************
//Запись данных из буфера 1 или 2 на произвольную страницу c предварительным заполнением буфера (командой группы Б) (для страницы 528 байт)
void PageProgramThroughBuffer512 (uint8_t NumberBuffer,uint16_t PageAddr, uint16_t BufferAddr)
{

    uint16_t i;
    
    if (NumberBuffer == 1)
        {
            SendCommand(MAIN_MEMORY_PAGE_THROUGH_BUFFER1);          //выбрать для записи буфер 1
        }
    else
        {
            SendCommand(MAIN_MEMORY_PAGE_THROUGH_BUFFER2);              //выбрать для записи буфер 2
        }
    SPI_PORT->PINRESET = DATA_OUT;                                  //передача двух незначащих бит
    OneClockEnable();
    OneClockEnable();
        
    SendBitToLine(13, PageAddr, 0xEFFF, 0x1000);                        //передача 13 бит адреса страницы   
    SendBitToLine(9, BufferAddr, 0xFEFF, 0x0100);                   //передача 9 бит начального адреса в буфере
     
    for(i=0; i<512;i++)
        {
            SendBitToLine(8, RWFlashBuffer512[i], 0xFF7F, 0x0080);      //передача 512-ми байт в буффер, по 8 бит
        }

}

//*************************************************************************************************
//Стирание сектора 0а и 0b (для страницы 528 байт)
void SendClearSectorAB528(uint8_t SectorAddr)
{
	volatile uint8_t i;
	uint16_t SendAddr, Temp_0;

	Temp_0 = SectorAddr;															//подготовка байта адреса к разборке на биты (адрес начального сектора+смещение) и сохранение байта адреса во временный регистр																													
	SendCommand(SECTOR_ERASE);													//передать команду стирания сектора
	SPI_PORT->PINRESET = DATA_OUT;										//передача первого незначащего бита 
	OneClockEnable();																		//защёлкнуть 1 бит в МС памяти			
	SendAddr = (~(0xFDFF) & Temp_0);									//запись в регистр передачи адреса сектора плюс его смещение
	for (i=0; i<10; i++)																//передача десяти бит адреса сектора (128 страниц по 528 байт)
        {
            if (SendAddr == 0x0200)
                    {
                        SPI_PORT->PINSET = DATA_OUT;
                    }
            else
                    {										
                        SPI_PORT->PINRESET = DATA_OUT;								
                    }
                    
            OneClockEnable();												//защёлкнуть 1 бит в МС памяти														
            Temp_0 = Temp_0<<1;		
            SendAddr = (~(0xFDFF) & Temp_0);		
                    
        }
        
    //SendBitToLine(10, Temp_0, 0xFDFF, 0x0200);    
        
	SPI_PORT->PINRESET = DATA_OUT;
	for (i=0; i<13; i++)															//передача 13-ти незначащих бит
        {
            OneClockEnable();												//защёлкнуть 1 бит в МС памяти
        }
		
}

//*************************************************************************************************
//Стирание сектора (1-63, для страницы 528 байт)
void SendClearSector528(uint8_t SectorAddr)
{
		volatile uint8_t i;
		uint16_t SendAddr;
		
		SendCommand(SECTOR_ERASE);													//передать команду стирания сектора
		SPI_PORT->PINRESET = DATA_OUT;										//передача первого незначащего бита 
		OneClockEnable();																		//защёлкнуть 1 бит в МС памяти			
		SendAddr = (~(0xFFDF) & SectorAddr);								//выделение шестого бита из байта адреса
		for (i=0; i<6; i++)																	//передача шести бит адреса сектора (128 страниц по 528 байт)
            {
                if (SendAddr == 0x0020)
                        {
                            SPI_PORT->PINSET = DATA_OUT;
                        }
                else
                        {										
                            SPI_PORT->PINRESET = DATA_OUT;								
                        }
                        
                OneClockEnable();												//защёлкнуть 1 бит в МС памяти														
                SectorAddr = SectorAddr<<1;		
                SendAddr = (~(0xFFDF) & SectorAddr);		
                        
            }
            
        //SendBitToLine(6, SectorAddr, 0xFFDF, 0x0020);   
            
		SPI_PORT->PINRESET = DATA_OUT;
		for (i=0; i<17; i++)															//передача 17-ти незначащих бит
            {
                OneClockEnable();												//защёлкнуть 1 бит в МС памяти
            }

}


//*************************************************************************************************
//Стирание сектора 0а и 0b (для страницы 512 байт)
void SendClearSectorAB512(uint8_t SectorAddr)
{
	volatile uint8_t i;
	uint16_t SendAddr, Temp_0;
		
	Temp_0 = SectorAddr;												//сохранение байта адреса во временный регистр																													
	SendCommand(SECTOR_ERASE);													//передать команду стирания сектора
	SPI_PORT->PINRESET = DATA_OUT;										//передача 2х незначащих бит 
	OneClockEnable();																		//защёлкнуть 1 бит в МС памяти
	OneClockEnable();																		//защёлкнуть 1 бит в МС памяти
	SendAddr = (~(0xFDFF) & Temp_0);									//запись в регистр передачи адреса сектора 
	for (i=0; i<10; i++)																//передача десяти бит адреса сектора (128 страниц по 528 байт)
        {
            if (SendAddr == 0x0200)
                    {
                        SPI_PORT->PINSET = DATA_OUT;
                    }
            else
                    {										
                        SPI_PORT->PINRESET = DATA_OUT;								
                    }
                    
            OneClockEnable();												//защёлкнуть 1 бит в МС памяти														
            Temp_0 = Temp_0<<1;		
            SendAddr = (~(0xFDFF) & Temp_0);		
                    
        }
        
    //SendBitToLine(10, Temp_0, 0xFDFF, 0x0200);    
        
	SPI_PORT->PINRESET = DATA_OUT;
	for (i=0; i<12; i++)															//передача 12-ти незначащих бит
        {
            OneClockEnable();												//защёлкнуть 1 бит в МС памяти
        }
				
}

//*************************************************************************************************
//Стирание сектора с 1-го по 63-й (для страницы 512 байт)
void SendClearSector512(uint8_t SectorAddr)
{
    volatile uint8_t i;
    uint16_t SendAddr;
        
    SendCommand(SECTOR_ERASE);													//передать команду стирания сектора
    SPI_PORT->PINRESET = DATA_OUT;										//передача 2х незначащих бит  
    OneClockEnable();																		//защёлкнуть 1 бит в МС памяти	
    OneClockEnable();																		//защёлкнуть 1 бит в МС памяти	
    SendAddr = (~(0xFFDF) & SectorAddr);								//выделение шестого бита из байта адреса
    for (i=0; i<6; i++)																	//передача шести бит адреса сектора (128 страниц по 528 байт)
        {
            if (SendAddr == 0x0020)
                    {
                        SPI_PORT->PINSET = DATA_OUT;
                    }
            else
                    {										
                        SPI_PORT->PINRESET = DATA_OUT;								
                    }
                    
            OneClockEnable();												//защёлкнуть 1 бит в МС памяти														
            SectorAddr = SectorAddr<<1;		
            SendAddr = (~(0xFFDF) & SectorAddr);		
                    
        }
        
    //SendBitToLine(6, SectorAddr, 0xFFDF, 0x0020);    
        
    SPI_PORT->PINRESET = DATA_OUT;
    for (i=0; i<16; i++)															//передача 16-ти незначащих бит
        {
            OneClockEnable();												//защёлкнуть 1 бит в МС памяти
        }


}

//*************************************************************************************************
//Передача команды
void SendCommand(uint8_t Command)
{
	volatile uint8_t i;
	uint16_t SendBit;	
	
	SendBit = ((~0xFF7F) & Command);	
	for (i=0; i<8; i++)
        {
            if (SendBit == 0x0080)
                    {
                        SPI_PORT->PINSET = DATA_OUT;
                    }
            else
                    {										
                        SPI_PORT->PINRESET = DATA_OUT;								
                    }

            OneClockEnable();												//защёлкнуть 1 бит в МС памяти           
            Command = Command<<1;					
            SendBit = (~(0xFF7F) & Command);		
                    
            //Command = Command<<1;					
            //SendBit = (~(0x7F) & (Command<<1));			
                                    
        }	

      //SendBitToLine(8, Command, 0xFF7F, 0x0080);

}

//*************************************************************************************************
//Установка начального адреса страницы и считываемого байта (для страницы 528 байт)
void StartAddrByteWrite528(uint16_t PageAddr, uint16_t StartByteAddr)
{
	volatile uint8_t i;
	uint16_t SendAddr;

	
	SendCommand(MAIN_MEMORY_PAGE_READ);									//команда чтения страницы	
	SendAddr = (~(0xEFFF) & PageAddr);								//выделение шестого бита из байта адреса
	for (i=0; i<13; i++)												//передача 13-ти бит адреса страницы (528 байт)
        {
            if (SendAddr == 0x1000)
                    {
                        SPI_PORT->PINSET = DATA_OUT;
                    }
            else
                    {										
                        SPI_PORT->PINRESET = DATA_OUT;								
                    }
                    
            OneClockEnable();										//защёлкнуть 1 бит в МС памяти														
            PageAddr = PageAddr<<1;		
            SendAddr = (~(0xEFFF) & PageAddr);		
                    
        }
	
    //SendBitToLine(13, PageAddr, 0xEFFF, 0x1000);   
        
	SendAddr = (~(0xFDFF) & StartByteAddr);							    //выделение шестого бита из байта адреса
	for (i=0; i<10; i++)											//передача 10-ти бит адреса байта на текущей странице (начало считывания страницы)
        {
            if (SendAddr == 0x0200)
                    {
                        SPI_PORT->PINSET = DATA_OUT;
                    }
            else
                    {										
                        SPI_PORT->PINRESET = DATA_OUT;								
                    }
                    
            OneClockEnable();											//защёлкнуть 1 бит в МС памяти														
            StartByteAddr = StartByteAddr<<1;		
            SendAddr = (~(0xFDFF) & StartByteAddr);		
                    
        }
        
    //SendBitToLine(10, StartByteAddr, 0xFDFF, 0x0200); 
        
	SPI_PORT->PINRESET = DATA_OUT;
	for (i=0; i<32; i++)											//передача 32-х незначащих бит (4 байта)
        {
            OneClockEnable();											//защёлкнуть 1 бит в МС памяти
        }
	
}

//*************************************************************************************************
//Установка начального адреса страницы и считываемого байта (для страницы 512 байт)
void StartAddrByteWrite512(uint16_t PageAddr, uint16_t StartByteAddr)
{
	volatile uint8_t i;
    uint16_t SendAddr;
	
	SendCommand(MAIN_MEMORY_PAGE_READ);									    //команда чтения страницы	
	SendAddr = (~(0xEFFF) & PageAddr);								    //выделение шестого бита из байта адреса
	for (i=0; i<13; i++)													//передача 13-ти бит адреса страницы (528 байт)
        {
            if (SendAddr == 0x1000)
                    {
                        SPI_PORT->PINSET = DATA_OUT;
                    }
            else
                    {										
                        SPI_PORT->PINRESET = DATA_OUT;								
                    }
                    
            OneClockEnable();												//защёлкнуть 1 бит в МС памяти														
            PageAddr = PageAddr<<1;		
            SendAddr = (~(0xEFFF) & PageAddr);		
                    
        }
        
    //SendBitToLine(13, PageAddr, 0xEFFF, 0x1000);
	
	SendAddr = (~(0xFEFF) & StartByteAddr);								//выделение шестого бита из байта адреса
	for (i=0; i<9; i++)													   //передача 9-ти бит адреса байта на текущей странице (начало считывания страницы)
        {
            if (SendAddr == 0x0100)
                    {
                        SPI_PORT->PINSET = DATA_OUT;
                    }
            else
                    {										
                        SPI_PORT->PINRESET = DATA_OUT;								
                    }
                    
            OneClockEnable();										    //защёлкнуть 1 бит в МС памяти														
            StartByteAddr = StartByteAddr<<1;		
            SendAddr = (~(0xFEFF) & StartByteAddr);		
                    
        }
     
    //SendBitToLine(9, StartByteAddr, 0xFEFF, 0x0100);
        
	SPI_PORT->PINRESET = DATA_OUT;
	for (i=0; i<32; i++)												    //передача 32-х незначащих бит (4 байта)
        {
            OneClockEnable();										    //защёлкнуть 1 бит в МС памяти
        }	

}

//*************************************************************************************************
//Отправка бит по SPI - параметры: кол-во бит, адрес байта, маска, число сравнения
void SendBitToLine(uint8_t NumberBit, uint16_t Addr, uint16_t Mask, uint16_t Compare)
{
    uint8_t i;
    uint16_t SendAddr = 0;
    
    SendAddr = (~(Mask) & Addr);								    //выделение шестого бита из байта адреса
    for (i=0; i<NumberBit; i++)										//передача 9-ти бит адреса байта на текущей странице (начало считывания страницы)
    {
        if (SendAddr == Compare)
                {
                    SPI_PORT->PINSET = DATA_OUT;
                }
        else
                {										
                    SPI_PORT->PINRESET = DATA_OUT;								
                }
                
        OneClockEnable();										    //защёлкнуть 1 бит в МС памяти														
        Addr = Addr<<1;		
        SendAddr = (~(Mask) & Addr);		
                
    }
}

//*************************************************************************************************
//Чтение байта с SPI выхода флешь
uint8_t ReadByteFromLine(void)
{
    volatile uint8_t i;
	uint32_t  Temp_0;	
	uint8_t ReadByte=0;
    
    //Temp_0 = (~(0xFFFBFFFF) & SPI_PORT->FIOPIN);	
	for (i=0; i<8; i++)										//приём 8-ми бит данных
        {
            OneClockEnable();								    //защёлкнуть 1 бит в МС памяти
            Temp_0 = (~(0xFFFBFFFF) & SPI_PORT->FIOPIN);	//выделение прочитанного бита
            
            if (Temp_0 == 0x40000)
                    {
                        ReadByte++;							    //прибавить 1 если считанный бит был лог. "1"
                    }					
            if (i<7)
                    {	
                        ReadByte = ReadByte<<1;				//сдвигать байт, пока не наберётся 8 прочитанных бит
                    }											
        }
        
    return(ReadByte);
}

//*************************************************************************************************
//Проверка флага занятости
void StatusRead(void)
{
	uint32_t ReadPort;
	
	ReadPort = SPI_PORT->FIOPIN;
	ReadPort = (~(0xFFF7FFFF) & ReadPort);
	while(ReadPort == 0x00)
			{
				ReadPort = SPI_PORT->FIOPIN;
				ReadPort = (ReadPort & (~0xFFF7FFFF));
			}
	
}

//*************************************************************************************************
//Сигнал тактирования
void OneClockEnable(void)
{
		SPI_PORT->PINSET = CLK;
		__NOP();		
		SPI_PORT->PINRESET = CLK;
		__NOP();
}



