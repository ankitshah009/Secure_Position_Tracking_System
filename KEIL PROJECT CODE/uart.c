
#include "config.h"


void Uart0Init (void)			    // Initialize Serial Interface       
{               	   
  	PINSEL0 |= 0x00000005;           //Enable RxD0 and TxD0                    
    U0LCR = 0x83;                   // 8 bits, no Parity, 1 Stop bit            
    U0DLL = 97;                     // 9600 Baud Rate @ 15MHz VPB Clock         
    U0LCR = 0x03; 					// DLAB = 0  
}

void Uart0PutCh (unsigned char ch)   // Write character to Serial Port   
{                  		
   while (!(U0LSR & 0x20));
  	U0THR = ch;
}

unsigned char Uart0GetCh (void)		// Read character from Serial Port   
{            
  while ((U0LSR&0x20) == 0);
  return (U0RBR);
}

void Uart1Init (void)			    // Initialize Serial Interface       
{               	   
  	PINSEL0 |= 0x00050000;           //Enable RxD1 and TxD1                     
    U1LCR = 0x83;                   // 8 bits, no Parity, 1 Stop bit            
    U1DLL = 97;                     // 9600 Baud Rate @ 15MHz VPB Clock         
    U1LCR = 0x03; 					// DLAB = 0  

}

void Uart1PutCh (unsigned char ch)   // Write character to Serial Port   
{                  		
  U1THR = ch; 	
	while ((U1LSR&0x20) == 0);
	

}

unsigned char Uart1GetCh (void)		// Read character from Serial Port   
{            
  while (!(U1LSR & 0x01));
  return (U1RBR);
}

void  Uart0PutS(unsigned char *str)	 //A function to send a string on UART0
{  
   while(*str)
   {  
      Uart0PutCh(*str++);	    
   }
}

void  Uart1PutS(unsigned char *str)	 //A function to send a string on UART1
{  
   while(*str)
   {  
      Uart1PutCh(*str++);
		 
   }
}
