/*Display - ERC12864*/
/*Controller - NT7538*/
#include "stm32f10x.h"
#include "Delay.h"
#include "ER-OLEDM024-1.h"
#include "ER-OLEDM024-1-12864SUMBOL.h"

			
//uint8_t i=0;	
volatile uint8_t PageCash = 0;


//************************************************************************************************
//������������� �������
void InitScreen (void)
		{

			SetPinsToOut(); 																//��������� ������������ �������� �������
			ScreenReset();																		//������� RESET
			Delay_nop();			
			CommandScreenPort->BSRR = DCCommandEnable;			//����� ������ �������
			
			ScreenWriteByte(DISPLAY_OFF);									//��������� ������� (?)			
			Delay_nop();																		//�����
				
			ScreenWriteByte(DIVIDE_RATIO_OSC_FREQUENCY);	//�������� 1, ������� ������ ���������� ��������
			ScreenWriteByte(DIVIDE1_FREQMAX_SET);						//
			Delay_nop();																	//�����
		
			ScreenWriteByte(MULTIPLEX_RATIO_SET);						//
			ScreenWriteByte(MULTIPLEX_64MUX);							//���������� 64 ��������
			Delay_nop();																		//�����
			
			ScreenWriteByte(DISPLAY_OFFSET_SET); 					//
			ScreenWriteByte(OFFSET); 												//
			Delay_nop();																	//�����
			
			ScreenWriteByte(MASTER_CONFIGURATION_SET);			//����� ���������	
			ScreenWriteByte(ENABLE_EXTERNAL_POWER_SUPPLY);//�������� ������� �� �������� ��������� �������
			Delay_nop();																			//�����
			
			ScreenWriteByte(COLOR_AND_LOWPOWER_MODE_SET);//���������� ��������������� �������, ����������� ��������, ������� ��������
			ScreenWriteByte(COLOR_AND_LOWPOWER_MODE);			//������� ����� ������ �����������
			Delay_nop();																			//�����
			
			ScreenWriteByte(SEGMENT_REMAP_COLUMN131_SEG0);	//
			ScreenWriteByte(SCAN_DIRECTION_COMn_COM0_SET);
			Delay_nop();																			//�����
			
			ScreenWriteByte(COM_PINS_SET);									//
			Delay_nop();																			//�����
			
			ScreenWriteByte(LOOK_UP_TABLE_SET);						//
			ScreenWriteByte(CURRENT_DRIVE_PULSE);						//
			ScreenWriteByte(CURRENT_DRIVE_PULSE);					//
			ScreenWriteByte(CURRENT_DRIVE_PULSE);						//
			ScreenWriteByte(CURRENT_DRIVE_PULSE);					//
			Delay_nop();																		//�����
					
			ScreenWriteByte(CONTRAST_CONTROL_BANK0_SET);				//
			ScreenWriteByte(CONTRAST_LEVEL_BANK0);									//
			Delay_nop();																				//�����
			ScreenWriteByte(BRIGHTNESS_AREA_COLOR_BANKS_SET);		//
			ScreenWriteByte(CONTRAST_LEVEL_BANKCOLOR);				//
			Delay_nop();																				//�����
			
			ScreenWriteByte(PRECHARGE_PERIOD_SET);				//
			Delay_nop();
			
			ScreenWriteByte(VCOMH_SET);										//��������� ���������� ��������� ����
			
			ScreenWriteByte(PAGE_START_END_ADDRESS_SET);
			ScreenWriteByte(0);
			ScreenWriteByte(7);
			Delay_nop();
			
			//ScreenWriteByte(PAGE_ADDRESSING_MODE);				//
			ScreenWriteByte(INVERSE_DISPLAY_PIXEL_SET);				//
			ScreenWriteByte(ENTIRE_DISPLAY_OFF);					//
			ScreenWriteByte(DISPLAY_ON_NORMAL_BRIGHTNESS);		//
	

			

		}

//************************************************************************************************
//������� �������	���������� ������������	
void ClearPageScreenProg (uint8_t Page)
		{

			uint8_t k=0;
			
			SetPage(Page);
			SetColumn(1);

					for (k=0;k<=127;k++) 
							
						{
							ScreenWriteData(0xFF);													
						}						
											
		}

//************************************************************************************************
//������� �������	���������� ������������	
void GroundPageScreenProg (uint8_t Page)
		{
					
			uint8_t k=0;
	
			SetPage(Page);
			SetColumn(1);
						
					for (k=0;k<=127;k++) //���-�� ����� � ������ 
							
						{
							ScreenWriteData(0x00);																					
						}
						
		}	
		
		
//************************************************************************************************
//������� �������	����������  - �������� ������ � ���������� ��������	
void GroundPageScreenHard (uint8_t Page)
		{
			
			ScreenWriteByte(ENTIRE_DISPLAY_ON);
			Delay_nop();				

		}
		
		
		
//************************************************************************************************
//������� �������	����������  - �������� ������ � ���������� ��������	
void ClearPageScreenHard (uint8_t Page)
		{

			ScreenWriteByte(ENTIRE_DISPLAY_OFF);
			Delay_nop();

		}			


//************************************************************************************************
//����� �������� �� �������, ��������(������ �������, � ������, 5 ������, 8 ������);  0-63 �����; 
//������� - ���������� ���, 0 - 127 �����; ���-�� �������� �� ������ 5 �� 8 �����	
void SendCharToScreen (uint8_t Page, uint8_t Position, uint8_t Quantity)
		{
					
			uint8_t k = 0;
			uint8_t QSymbol = 0;
			uint8_t i = 0;
			
			SetPage(Page);
			SetColumn(Position);
			
			
			
			QSymbol = (CharPoints*Quantity)-1;				//���-�� �����, ������������ �� ��������� ���-�� ��������
			if (Quantity==0){QSymbol=0;}
				
					for (k=0;k<=QSymbol;k++) //���-�� ����� � ������ 
							
						{

							ScreenWriteData(Font[i++]);
					
							if (i==54) {i=0;}													//���-�� ��������� � �������
																
						}

						SetColumn(1);

		}		
		
//************************************************************************************************
//�������� ���������� ��������� �� �������							
void SendText(uint8_t *ASCII_Code, uint8_t Page, uint8_t Position)
{
		uint8_t i,j;
	
   for(i=0;i<9;i++)
  {
    //LCDSendChar(ASCII_Code[i]);
		SendOneChar(Page, Position, ASCII_Code[i]);
		j++;
  }
}		
		
//************************************************************************************************
//����� ������� �� ������� 
//������� - �������������� �� ����������� 	
void SendOneChar (uint8_t Page, uint8_t Position, uint8_t ASCII_Code)
		{
					
			uint8_t k = 0;
			uint8_t Temp_0 = 0;
			uint8_t QSymbol = 0;
		
			SetPage(Page);																			//��������� �������� ������ �������	
			SetColumn(Position);																	//��������� ��������� ������� (�� �����������)
			
			//QSymbol = CharPoints - 1;			
			QSymbol = 4;																				//���-�� �����, ������������ �� ��������� ���-�� �������� (������ �������)
			Temp_0 = SCREEN_EN_RUS_ASCI_WIN1251_v1[ASCII_Code];		//��������� �������� ��� ������� �������� �� ���������� ���� ASCII WIN1251																		

						for (k=0;k<=QSymbol;k++) 											//���-�� �����, �� �������� ������� ������ (���-�� ���� �� ���������, �.�. � ������� ������������ ���������) 
								
								{										
									ScreenWriteData(Font[Temp_0++]); 					//�� ������� ���� ����� �������� ��������, �� ����������� �������� �� ������� ��������
								}

			SetColumn(1);																				//������� ��������� �� ����������� � �������� ���������

		}
		
		

		
//************************************************************************************************
//��������� ������� �������� �������		
void SetPage (uint8_t Page)
		{
			
			ScreenWriteCommand(PAGE1_ADDRESS_SET+(Page-1));
			
			//CommandScreenPort->BSRR = DCCommandEnable;			//����� ������ �������
			//ScreenWriteByte(PAGE1_ADDRESS_SET+(Page-1));			//���������� ��������� ��������, ����� 1 ��� ���������� ������� ������� � �������� ����
			//Delay_nop();//��� ������������� ����������������� ������� ������ ��� �������� �� �����
		}
	
//************************************************************************************************
//��������� �������� �������(���������) �������		
void SetColumn (uint8_t Column)
		{
			CommandScreenPort->BSRR = DCCommandEnable;												//����� �������
			ScreenWriteByte(COLUMN_START_ADDRES_SET_HIGH+((Column+1)>>4));				//������������ ������ ��������, ���������� �
			ScreenWriteByte(COLUMN_START_ADDRES_SET_LOW+((Column+1) & (~0xF0)));		
			Delay_nop();//��� ������������� ����������������� ������� ������ ��� �������� �� �����	
		}
		

		
//************************************************************************************************
//������� ��������� ������� �� ����� (���������, ��� ����� �������)	
void SetPinsToOut (void)			
			{

      DataLineScreenPort->CRL	&= ~(GPIO_CRL_CNF0 | GPIO_CRL_CNF1 | GPIO_CRL_CNF2 | GPIO_CRL_CNF3 | GPIO_CRL_CNF4 | GPIO_CRL_CNF5 | GPIO_CRL_CNF6 | GPIO_CRL_CNF7);			//����� CNF � 00								
	    DataLineScreenPort->CRL  |= (GPIO_CRL_MODE0 | GPIO_CRL_MODE1 | GPIO_CRL_MODE2 | GPIO_CRL_MODE3 | GPIO_CRL_MODE4 | GPIO_CRL_MODE5 | GPIO_CRL_MODE6 | GPIO_CRL_MODE7);	//��������� ������ ��-GP

      CommandScreenPort->CRH	&= ~(GPIO_CRH_CNF9 | GPIO_CRH_CNF10 | GPIO_CRH_CNF11 | GPIO_CRH_CNF12 | GPIO_CRH_CNF15);			//����� CNF � 00								
	    CommandScreenPort->CRH  |= GPIO_CRH_MODE9 | GPIO_CRH_MODE10 | GPIO_CRH_MODE11 | GPIO_CRH_MODE12| GPIO_CRH_MODE15;	//��������� ������ ��-GP
			
			CommandScreenPort->BSRR = ResetDisable;						//����� ������� �� �������
			CommandScreenPort->BSRR = CSDisable;					  //������� �� �������
			CommandScreenPort->BSRR = WRDisable;					  	//����� ������ �� �������
			CommandScreenPort->BSRR = RDDisable;					  //����� ������ �� �������
			
				
			}
			
			
//************************************************************************************************
//������� ������ ������� ����� ������ ������� 
void ScreenReset (void)
		{
			
			CommandScreenPort->BSRR = ResetDisable;						//����� ������� �� �������
			Delay_ms(50);																	//����� ����� ������ ������� �� �������
			CommandScreenPort->BSRR = ResetEnable;						//����� ������� �������
			Delay_ms(1);																	//����� ����� ������ ������� �� �������
			CommandScreenPort->BSRR = ResetDisable;						//����� ������� �� �������	
			Delay_ms(1);																		//����� ����� ������� �����		
			CommandScreenPort->BSRR = CSEnable;					  		//��������� �������
			Delay_us(1);																		//������� �������� �� �������  NOP (������� �� ������� ����)
     			
		}
			
//************************************************************************************************
//������� ������� ������� ������� 	
void ScreenWriteCommand (uint8_t Command)
		{		
				CommandScreenPort->BSRR = DCCommandEnable;//����� �������
				ScreenWriteByte(Command);
				//Delay_nop();
		}	

//************************************************************************************************
//������� �������� ������ �� ������� 
void ScreenWriteData (uint8_t Data)
		{		
				CommandScreenPort->BSRR = DCDataEnable;//����� �������
				ScreenWriteByte(Data);
				//Delay_nop();
		}


//************************************************************************************************
//������� ������ ����� � ���� �� (����������������, ���� ������������ ����� �� i8080) 
void ScreenWriteByte (uint8_t Byte)
		{
		
			DataLineScreenPort->ODR &= ~0x00FF;									//������� ������� 8 ��� �����
			DataLineScreenPort->ODR |= Byte;									//�������� � ����  
			
			CommandScreenPort->BSRR = WREnable;					  			//����� ������ �������
			//CommandScreenPort->BSRR |= CEEnable;					  		//������� �������
			Delay_nop();																			//������� �������� �� �������  NOP (������� �� ������� ����)	
			//Delay_us(10);
			//CommandScreenPort->BSRR |= CEDisable;					  		//������� �� �������
			CommandScreenPort->BSRR = WRDisable;					  	//����� ������ �� �������
			Delay_nop();
			DataLineScreenPort->ODR &= ~0x00FF;									//������� ������� 8 ��� �����
			
			
		}


////************************************************************************************************
////������� ������ ����� � ���� �� (����������������, ���� ������������ ����� �� SPI)
//void ScreenWriteByte (uint8_t Byte)
//		{
//		
//				volatile uint8_t i;
//				uint8_t SendByte;
//			
//				SendByte = (~(0x7F) & Byte);
//					
//				for (i=0; i<8; i++)
//						{
//							if (SendByte == 0x80)
//									{
//										DataLineScreenPort->BSRR = DataHigh;
//									}
//							else
//									{										
//										DataLineScreenPort->BSRR = DataLow;
//										
//									}
//				
//							DataLineScreenPort->BSRR = ClockHigh;
//							Delay_nop ();
//							DataLineScreenPort->BSRR = ClockLow;
//							Delay_nop ();
//							//DataLineScreenPort->BSRR = DataLow;
//							
//							Byte = Byte<<1;					
//							SendByte = (~(0x7F) & Byte);			
//													
//						}
//		
//		}

//************************************************************************************************
//������� �������� �� �������  NOP (������� �� ������� ����)
void Delay_nop (void)
		{
		
		__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();
		//__NOP();__NOP();__NOP();
		//__NOP();__NOP();__NOP();
		//__NOP();__NOP();__NOP();
		//__NOP();__NOP();__NOP();
		
		}	
		


//************************************************************************************************
//������� ��������� ������� �� ����� (���������, ��� ����� �������)	
void SetPinsToOut_ (void)			
			{

      DataLineScreenPort->CRL	&= ~(GPIO_CRL_CNF0 | GPIO_CRL_CNF1 | GPIO_CRL_CNF2 | GPIO_CRL_CNF3 | GPIO_CRL_CNF4 | GPIO_CRL_CNF5 | GPIO_CRL_CNF6 | GPIO_CRL_CNF7);			//����� CNF � 00								
	    DataLineScreenPort->CRL  |= (GPIO_CRL_MODE0 | GPIO_CRL_MODE1 | GPIO_CRL_MODE2 | GPIO_CRL_MODE3 | GPIO_CRL_MODE4 | GPIO_CRL_MODE5 | GPIO_CRL_MODE6 | GPIO_CRL_MODE7);	//��������� ������ ��-GP
																
			}
			
//************************************************************************************************
//������� ��������� ������� �� ���� (���������, ��� ����� �������)	
void SetPinsToIn_ (void)			
			{

      DataLineScreenPort->CRL	&= ~(GPIO_CRL_CNF0 | GPIO_CRL_CNF1 | GPIO_CRL_CNF2 | GPIO_CRL_CNF3 | GPIO_CRL_CNF4 | GPIO_CRL_CNF5 | GPIO_CRL_CNF6 | GPIO_CRL_CNF7);			//����� CNF � 00
			DataLineScreenPort->CRL	|=  (GPIO_CRL_CNF0_1 | GPIO_CRL_CNF1_1 | GPIO_CRL_CNF2_1 | GPIO_CRL_CNF3_1 | GPIO_CRL_CNF4_1 | GPIO_CRL_CNF5_1 | GPIO_CRL_CNF6_1 | GPIO_CRL_CNF7_1);							//����� CNF � 00	
	    DataLineScreenPort->CRL  &= ~(GPIO_CRL_MODE0 | GPIO_CRL_MODE1 | GPIO_CRL_MODE2 | GPIO_CRL_MODE3 | GPIO_CRL_MODE4 | GPIO_CRL_MODE5 | GPIO_CRL_MODE6 | GPIO_CRL_MODE7);	//��������� ������ ��-GP
		
			}	

//************************************************************************************************
//������� ������ ����� � ���� �� 

uint8_t ScreenDataRead (void)
		{
			uint8_t Temp_0;
			
			CommandScreenPort->BSRR = DCCommandEnable;	//����� �������
			ScreenWriteByte(READ_MODIFY_WRITE_ON);		//�������� ����� "������-�����������-������"
			SetPinsToIn_();															//��������� ������� �� �� ���� ��� ����� ������
			CommandScreenPort->BSRR = DCDataEnable;		//����� ������
			CommandScreenPort->BSRR = RDEnable;					//����� ������ ������ �������   //��� ������ ������, ������ ������ �� ������������
			//__NOP();
			CommandScreenPort->BSRR = RDDisable;			//����� ������ ������ �� �������	//� ��� �������� ���������� �������� ����� ������������ ����, ��� �������� �� ������
			CommandScreenPort->BSRR = RDEnable;					//����� ������ ������ �������
			
			Delay_nop ();															//�����
			Temp_0 = (DataLineScreenPort->IDR);						//���� ������ �� �������
			CommandScreenPort->BSRR = RDDisable;			//����� ������ ������ �� �������				
			SetPinsToOut_();														//��������� ������� �� �� ����� ��� �������� ������
			
			return(Temp_0);
	
		}

//************************************************************************************************
// ��������� ��������� �������
void PixelSetReset (uint8_t Horizontal, uint8_t Vertical, uint8_t SetClrBit)
		{
			uint8_t BitNumber =0, PageNumber =0, NumbPicsel =0, Temp_0 = 0;
		
			
			if (Vertical<PageWidth)
					{
						BitNumber = Vertical;																//����� ����
						PageNumber = 1;																				//����� ��������					
					}
			else
					{
									
							BitNumber = (Vertical - (8*(Vertical/PageWidth)));//���������� ������ ����, PageWidth = 8
							
							if (BitNumber == 0)																	//���� 0, �� ����� ���������� ������� �����
									{
										BitNumber = PageWidth;											//����� ���� = ���������� �� ��������� (y) ������ �� ������ ��������
										PageNumber = Vertical/PageWidth;							//����� ��������
									}
									
							else 
									{
										Temp_0 = Vertical/PageWidth;								//����� ���� = ���������� �� ��������� (y) ������ �� ������ ��������
										PageNumber = 	Temp_0 + 1;											//����� ��������							
									}
						}
						
			NumbPicsel = NumberPicselArr[BitNumber-1];								//����������� ����� � ������ �������� �� ������ ���������� ����
				
			SetColumn(Horizontal);																			//���������� ����� ��������
			SetPage(PageNumber);																			//���������� ����� ��������					

			if (SetClrBit == 1)																				//���� �������, �� ������� ���������������
					{	
						Temp_0 = (ScreenDataRead() & (~NumbPicsel));				//��������� ��������� ������ � ���������������� ����
					}
			else																												//����� ������� ��������� 
					{
						Temp_0 = (ScreenDataRead() | NumbPicsel);						//��������� ��������� ������ � ���������� ����
					}
			
			ScreenWriteByte(Temp_0);																		//������ ����������������� �������
			ScreenWriteCommand (READ_MODIFY_WRITE_OFF);								//��������� ����� "������-�����������-������"
					
		}	 

//************************************************************************************************
//	
void Line (uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2)
		{
			
			uint8_t x = 0, y =0;
			
			
			
				for (y=Y1;y<=Y2;y++)
							{
									for (x=X1;x<=X2;x++)
								
											{
												
												Delay_ms(100);
												PixelSetReset (x,y,1);
																						
											}
											
//										for (x=X1;x<=X2;x++)
//								
//											{
//												
//												Delay_ms(100);
//												PixelSetReset (x,y,0);
//																						
//											}		
							//if (y == Y2) { y = Y1;}							
							
							}
		}		
		



