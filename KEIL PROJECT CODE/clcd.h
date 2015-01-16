//************************************************************************************
//
//                  LCD 16X2 (CLCD) Interface Library (Header File)
//
//************************************************************************************

#ifndef CLCD_H
#define CLCD_H    

//*****USER SETTINGS************
#define DATA_PORT_SET       IOSET0
#define DATA_PORT_CLR       IOCLR0
#define DATA_DIR            IODIR0
#define D7					20
#define D6					19
#define D5					18
#define D4					17
//Set data port pins
#define DATA_PORT    		(unsigned long)((1<<D7)|(1<<D6)|(1<<D5)|(1<<D4))

#define CTRL_PORT_SET       IOSET0
#define CTRL_PORT_CLR       IOCLR0
#define CTRL_DIR            IODIR0
#define CTRL_RS             12
#define CTRL_EN             13
//***End of USER SETTINGS*******    
  
//*******Public Protoype**********
#define ClcdPutCh(ch)       ClcdSendByte(ch,1)
#define ClcdCursorOn()      ClcdSendByte(0x0E,0)
#define ClcdCursorOff()     ClcdSendByte(0x0C,0)
#define ClcdDisplayOff()    ClcdSendByte(0x08,0)
#define ClcdDisplayOn()     ClcdSendByte(0x0C,0)
#define ClcdCursorBlink()   ClcdSendByte(0x0F,0)

void ClcdInit(void);
void ClcdSendByte(unsigned char byte,unsigned char type);
void ClcdGoto(unsigned char x,unsigned char y);
void ClcdPutS(void *str);
void ClcdPutS_P(const char *str);
void ClcdPutNum(int n);
void ClcdPutFloat(float f);
void ClcdClear(void);
//*****End of Public Protoype//*****

#endif  //CLCD_H
