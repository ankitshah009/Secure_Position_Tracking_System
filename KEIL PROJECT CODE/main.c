
//	PINOUT:			LCD   -   LPC2148 
//					----------------- 
//					DB4   -   	P0.17 
//					DB5   -   	P0.18 
//					DB6   -   	P0.19
//					DB7   -   	P0.20 
//					RS    -   	P0.12 
//					E     -   	P0.13 
//					R/W   -   	GND
//***************************************************************************

//**************************INCLUDES********************************************
#include "config.h"
#include<string.h>
//**********************End of INCLUDES***************************************** 

void _DelayMs(unsigned int);
unsigned char GsmSendMsg(unsigned char *msgStr);
unsigned char GsmReadMsg(unsigned char *msgstr);
void _DelayMs(unsigned int count);
void reset();
int handle_byte(int bytegps);

int counter1=0;
int counter2=0;
int bytegps;
char cmd[7]="$GPRMC";
char lng[25]=" Long:";
char lat[7]="Lat:";
int offsets[13];
char buffer[300]="";
char data1[20]="";
char data2[20]="";
char unuser[50]="Unauthorised User no : ";
char lastfive[100]="";
int offset;
int i=0,j=0,cp=0;
unsigned char msg;
int length=0;
int k=200;
int a=0,b=7,c=7;
int count0a=0;
int countcomma=0;
int count_lf=0;
unsigned char usrNumStr[20]="\"+919035220425\"";//Receiptent Mobile Number
unsigned char msgno='1';
unsigned char rcmsg[150]="";
unsigned char rcmsg1[20]="";
unsigned char rcmsg2[20]="";

unsigned char msgformat[20]="position";//codeword-1
unsigned char msgformat1[20]="lastfive";//codeword-2



int main(void)
{
	IODIR1|=0xFFFF0000;			
	IOPIN1&=~(0xFFFF0000);
	
    Uart0Init();
	  Uart1Init();
    ClcdInit();
    ClcdCursorOff(); 
    ClcdPutS("REAL TIME POSITN");
    ClcdGoto(1,2);
    ClcdPutS("    TRACKING!    ");  
    _DelayMs(500);
    ClcdClear();
    ClcdPutS(" GETTING READY");
	  _DelayMs(500);
	  ClcdClear();
    ClcdGoto(1,2);
    ClcdPutS("     DONE!!");

    //  Echo off
    Uart1PutS("ATE0\r");
    _DelayMs(500);
   
    //  For getting sms length during sms read      
    Uart1PutS("AT+CSDH=1\r");
    _DelayMs(500);

    
	while(1)
	{
	  bytegps=Uart0GetCh();
		if (!handle_byte(bytegps))
     reset();
	}
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
		ClcdClear();
		ClcdPutS("SENDING MSG");
		_DelayMs(500);
		 
		Uart1PutS("AT+CMGS=");
    Uart1PutS(usrNumStr);
    Uart1PutCh('\r');
    _DelayMs(100);  
    Uart1PutS(msgStr);
    _DelayMs(100);  
    Uart1PutCh(0x1A);
    _DelayMs(500); 
		Uart1PutS("AT+CMGD=1\r");
    _DelayMs(500);
	   Uart1PutCh(0XD);
		ClcdClear();
		ClcdPutS("MSG SENT");
		_DelayMs(500);
		ClcdClear();
		ClcdPutS("DELETING MSG");
		_DelayMs(500);
		ClcdClear();
		Uart1PutS("AT+CMGD=1\r");
		_DelayMs(500);
		Uart1PutCh(0XD);
		reset();
			return 1;
		
}


void reset()
{
	counter1=0;
	counter2=0;
	a=0;
	b=7;
	c=7;
	k=200;
	i=0;
	j=0;
	return;
}

int get_size(int offset)
 {
    return offsets[offset+1]-offsets[offset]-1;
 }
 
int handle_byte( int bytegps)
   {	char lat[7]="Lat:";	
		 char lng[25]="Long:";
		 char unuser[50]="Unauthorised User no : ";
		 
				buffer[counter1]=bytegps;
				counter1++;
					
		 
								if(counter1==300)
								{
									return 0;
								}
			
								if (bytegps==',')
								{  
									counter2++;
									offsets[counter2]=counter1;
									
												if (counter2==13)
												{
													return 0;
												}
								}//if bytegps==',' ends
								
								
									if (bytegps=='*')
									{
										
										offsets[12]=counter1;
									}//if bytegps=='*' ends
									
									
								if (bytegps==0x0a)
									{						
									if(counter2!=12 || (get_size(0)!=6))
									{
										return 0;
									}

									
									for(j=0;j<6;j++)
									{ 										
													if (buffer[j] != cmd[j])
													{
														return 0;
													}			
									}
					
					//second case in if bytegps==' '
									if (get_size(3) != 9)
										{
								
										return 0;
										}
										
										
					//third case in if bytegps==' '
										if (get_size(5)!=10)
										{
											return 0;
										}
					
					
					for (j=0;j<9;j++)
					{
						data1[j]=buffer[offsets[3]+j];
					}
					
					for (j=0; j<10;j++)
					{
					data2[j]=buffer[offsets[5]+j];
						
					}
					 ClcdClear();
					ClcdGoto(1,1);
					ClcdPutS("Lat:");
					ClcdPutS(data1);
					ClcdGoto(1,2);
					ClcdPutS("Long:");
					ClcdPutS(data2);
					_DelayMs(1000);
					ClcdClear();
					count_lf++;
					if(count_lf % 300==1)
					{
						strcat(lastfive,lat);
						strcat(lastfive,data1);
						strcat(lastfive,lng);
						strcat(lastfive,data2);
						if (count_lf==1500)
						{count_lf=0;
						memset(lastfive,0,strlen(lastfive));	
						}
				}
				
					GsmReadMsg("1");
				
					if (a==15 & b==15)
					{	
						strcat(lat,data1);
						strcat(lat,lng);
						strcat(lat,data2);
						GsmSendMsg(lat);
						ClcdClear();
					ClcdPutS("RESETTING");
					_DelayMs(500);
					ClcdClear();
						memset(lat,0,strlen(lat));
					}
					
					if(a!=15 & b==15)
					{	
						strcat(unuser,rcmsg1);
						GsmSendMsg(unuser);
						memset(unuser,0,strlen(unuser));
					}
					if(a==15 & c==15)
					{
						GsmSendMsg(lastfive);
						
					}
					
					return 0;
				} 
				return 1;
			}

unsigned char GsmReadMsg(unsigned char *msgstr)
{   int counter0a=0;
					
					ClcdClear();
					ClcdPutS("READING MSG");
					_DelayMs(500);
	Uart1PutS("AT+CMGR=");
		Uart1PutCh(msgno);
      _DelayMs(500);
	  Uart1PutCh(0XD); 
	

		i=0;
	k=200;
	count0a=0;
	while(k>0)
	{	
	  msg=Uart1GetCh();
		rcmsg[i]=msg;
		i++;
		if (msg==0x0a)
		{
			count0a++;
		}
		if (count0a==5)
		{k=0;
			count0a=0;
		}
		if (count0a==2)
			if (i<50)
			{
				ClcdClear();
					ClcdPutS("ERROR");
					_DelayMs(100);
					count0a=0;
				return;
			}
		k--;
	}
	
		
		length=strlen(rcmsg);
		j=0;
		countcomma=0;
		for(i=0;i<length;i++)
		{		Uart0PutCh(rcmsg[i]);
			if(rcmsg[i]>96 & rcmsg[i]<123)
			{
				rcmsg2[j]=rcmsg[i];
				j++;
			}
			
			if (rcmsg[i]==44)
				countcomma=countcomma+1;
			
			if(countcomma==0)
			{
			for(k=0;k<15;k++)
					rcmsg1[k]=rcmsg[i+k+2];
			}
			
		}
		
		_DelayMs(500);
		ClcdClear();
		ClcdGoto(1,1);
		ClcdPutS("RECEIVED MSG:");
		ClcdGoto(1,2);
		ClcdPutS(rcmsg2);
		_DelayMs(1000);
		
		ClcdClear();
		ClcdGoto(1,1);
		ClcdPutS("SENDERS NO:");
		ClcdGoto(1,2);
		ClcdPutS(rcmsg1);
		_DelayMs(1000);
		
		a=0;
		for(j=0;j<15;j++)
		{
			if(usrNumStr[j]==rcmsg1[j])
				a++;
		}
		b=7;
		length=strlen(rcmsg2);
		for(i=0;i<length;i++)
		{
			if(msgformat[i]==rcmsg2[i])
	   	b++;
		}
		c=7;
		for(i=0;i<length;i++)
		{
			if(msgformat1[i]==rcmsg2[i])
	   	c++;
		}
    memset(rcmsg1,0,strlen(rcmsg1));
    memset(rcmsg2,0,strlen(rcmsg2));		
	
    _DelayMs(30); 
    return 1;
}

