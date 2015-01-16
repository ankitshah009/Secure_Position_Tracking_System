*

//**************************INCLUDES********************************************
#include "config.h"
//**********************End of INCLUDES***************************************** 

//************************Private Protoype**************************************
void _DelayMs(unsigned int);
unsigned char GsmSendMsg(unsigned char *msgStr);
void _DelayMs(unsigned int count);
//*******************End of Private Protoype************************************

//***********************Variable Declarations**********************************
unsigned char usrNumStr[20]="\"+918951805494\"";	//Receiptent Mobile Number
//*******************End of Variable Declarations******************************

int main(void)
{
	IODIR1|=0xFFFF0000;			//Turn Off the LEDs on board
	IOPIN1&=~(0xFFFF0000);
	
    Uart0Init();
    Uart0PutS("ATE0\r");
    _DelayMs(500);
   
    //  For getting sms length during sms read      
    Uart0PutS("AT+CSDH=1\r");
    _DelayMs(500);

    GsmSendMsg("Message from Entesla's ARM7 Dev. board");
    
	while(1);

}

void _DelayMs(unsigned int count) 
{
    volatile unsigned int j,k;
    for (j=0;j<count;j++) 
	{
        for (k=0;k<6000;k++) 
		{
            __asm 
			{
                nop;
                nop;
            }
        }
    }
}


unsigned char GsmSendMsg(unsigned char *msgStr)
{
    Uart0PutS("AT+CMGS=");
    Uart0PutS(usrNumStr);
    Uart0PutCh('\r');
    _DelayMs(100);  // timepass
    Uart0PutS(msgStr);
    _DelayMs(100);  // timepass
    Uart0PutCh(0x1A);
    _DelayMs(3000);  // timepass
    return (1);
}

