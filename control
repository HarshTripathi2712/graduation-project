#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/UART.h"
#include "inc/hw_uart.h"
#include <string.h>
#include <math.h>



#define GPIO_PA0_U0RX           0x00000001
#define GPIO_PA1_U0TX           0x00000401
#define GPIO_PB0_U1RX           0x00010001
#define GPIO_PB1_U1TX           0x00010401
#define GPIO_PD6_U2RX           0x00031801
#define GPIO_PD7_U2TX           0x00031C01


unsigned int Px_1m =65;
const char *MacAddr[4] = {"D8B04CB4F2C2  RSSI:","D8B04CB4F2BA  RSSI:","tbd","tbd"};

char BUF[400];
unsigned int BUFindex = 0;
unsigned int start = 0;
unsigned int buttonflag = 0;


#define Second 4000000
#define MilliSecond 4000
#define MicroSecond 4
unsigned long TheSysClock=12000000UL;


void delay_s(int time)
{
	time=time*Second;
	while((time--)>0);
}
void delay_ms(int time)
{
	time=time*MilliSecond;
	while((time--)>0);
}
void delay_us(long int time)
{
	time=time*MicroSecond;
	while((time--)>0);
}

int find(char *text,char *find)
{
	if(*text == '\0' || *find =='\0')
		return -1;
	int l_text=strlen(text);
	int l_find=strlen(find);
	if(l_text < l_find)
		return -1;
	char *s = text;
	char *p = s;
	char *q = find;
	while(*p != '\0')
	{
		if(*p == *q)
		{
			p++;
			q++;
		}
		else
		{
			s++;
			p = s;
			q = find;
		}
		if(*q == '\0')
			return p - text;
	}
	return -1;
}




void UartPutstringToUART0(char *ptr)
{
    while(*ptr != '\0')
    {
    	UARTCharPut(UART0_BASE,*(ptr++));                  // 发送数据
    }
    UARTCharPut(UART0_BASE,'\n');                        //发送换行指令
}

void UartPutstringToUART1(char *ptrr)
{
    while(*ptrr != '\0')
    {
    	UARTCharPut(UART1_BASE,*(ptrr++));                  // 发送数据
    }
    UARTCharPut(UART1_BASE,'\n');                        //发送换行指令
}

char *F2S(float d, char* str)
{
	char str1[10];
    int j=0,k,i;
    i = (int)d;  //浮点数的整数部分
    while(i > 0)
    {
    	str1[j++] = i%10+'0';
    	i /= 10;
    }
    for(k=0;k<j;k++) str[k] = str1[j-1-k]; //
    str[j++] = '.';
    d -= (int)d;

    for(i=0;i<2;i++)
 	{
    	d *= 10;
    	str[j++] = (int)d+'0';
    	d -= (int)d;
 	}
    str[j] = '\0';
    return str;
}

void GetRssi(int rssi[])
{
	UartPutstringToUART1("at+mode=m\r");
	UartPutstringToUART1("at+scan\r");
	delay_ms(2000);

	//IE2 &= ~UCA0RXIE;
	UartPutstringToUART1("at+z\r");
	delay_ms(1000);
	BUF[BUFindex]='\0';
	UartPutstringToUART1(BUF);

	int index[4];
	int i;
	char tmp[30];
	for(i=0;i<4;i++)
	{
		strcpy(tmp,MacAddr[i]);
		index[i]=find(BUF,tmp);
		if(index[i] != -1)
		{
			UartPutstringToUART0(tmp);
			UARTCharPut(UART0_BASE,BUF[index[i]]);
			UARTCharPut(UART0_BASE,BUF[index[i]+1]);
			UARTCharPut(UART0_BASE,BUF[index[i]+2]);
			UartPutstringToUART0(" dbm");
			rssi[i] = 10 * (BUF[index[i]+1] - '0') + BUF[index[i]+2] - '0' ;
		}
		else
		{
			rssi[i] = 0;
			if(find(tmp,"tbd") == -1)
			{
				UartPutstringToUART0(tmp);
				UARTCharPut(UART0_BASE,'0');
				UartPutstringToUART0(" dbm");
			}
		}
	}
	strcpy(BUF,"");
	BUFindex = 0;
	start = 0;
	//IE2 |= UCA0RXIE;
}


void GetXY(float xy[],float d[],int a)
{
	float cos1,sin1,tmp;
	if (d[0] != 0 && d[1] != 0 )
	{
		tmp=(d[0]*d[0] + a*a - d[1]*d[1])/(2*d[0]*a);
		cos1 = tmp;
		sin1 = sqrt(1 - tmp * tmp);
		xy[0] = d[0] * cos1;
		xy[1] = d[0] * sin1;
	}
}

void GetDistance(float distance[],int rssi[],int px_1m)
{
	float p[2];
	int i=0;
	for(i=0;i<2;i++)
	{
		if(rssi[i] != 0)
		{
			p[i]=(rssi[i]-px_1m)/39.8;
			distance[i]= 1 + 2.3 * p[i] + 2.65 * p[i] * p[i];
		}
		else distance[i]=0;
	}
}



int main(void)
{


	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE,GPIO_PIN_1|GPIO_PIN_2);
	GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE,GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

//-------------------------------------------------------------------------------------------------------------------------


//-------------------------------UART0配置----------------------------------------------------------------------------------
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE,GPIO_PIN_0|GPIO_PIN_1);
	UARTConfigSetExpClk(UART0_BASE,SysCtlClockGet(),57600,(UART_CONFIG_WLEN_8|UART_CONFIG_STOP_ONE|UART_CONFIG_PAR_NONE));
//-------------------------------UART1配置-----------------------------------------------------------------------------------
		SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
		GPIOPinConfigure(GPIO_PB0_U1RX);
		GPIOPinConfigure(GPIO_PB1_U1TX);
		GPIOPinTypeUART(GPIO_PORTB_BASE,GPIO_PIN_0|GPIO_PIN_1);
		UARTConfigSetExpClk(UART1_BASE,SysCtlClockGet(),57600,(UART_CONFIG_WLEN_8|UART_CONFIG_STOP_ONE|UART_CONFIG_PAR_NONE));
//----------------------------------------------------------------------------------------------------------------------------

	IntEnable(INT_UART1);
	IntEnable(INT_UART0);


	UARTIntEnable(UART0_BASE,UART_INT_RX|UART_INT_RT);

	UARTIntEnable(UART1_BASE,UART_INT_RX|UART_INT_RT);


	IntMasterEnable();

	UARTEnable(UART0_BASE);
	UARTEnable(UART1_BASE);
	UartPutstringToUART0(" abc");
	UartPutstringToUART1(" abc");
	UARTCharPut(UART0_BASE,'!');
	UARTCharPut(UART1_BASE,'!');
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 4);

		float d[2];
		float xy[2];
		int r[2];
		unsigned int i;
		char tmp[10];

	while(1)
	{
		if(buttonflag == 1)
		    	{
		    		buttonflag = 0;
		    		if(start == 0)
		    		{
		    		    UartPutstringToUART1("+++a");
		    		    delay_ms(20);
		    		    if((find(BUF,"a+ok") != -1) || (find(BUF,"ERROR") != -1))
		    		   {start = 1;
		    		    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 8);}
		    		    strcpy(BUF,"");
		    		    BUFindex = 0;
		    		}
		    		else
		    		{
		    			GetRssi(r);
		    			GetDistance(d,r,Px_1m);
		    			GetXY(xy,d,2);
		    			for(i = 0;i < 2;i++) UartPutstringToUART0(F2S(d[i],tmp));
		    			UartPutstringToUART0(F2S(xy[0],tmp));
		    			UartPutstringToUART0(F2S(xy[1],tmp));
		    		}
		    	}
	}
}

void UART1IntHandler(void)                         //接收中断
{

	UARTIntClear(UART1_BASE,UART_INT_RX|UART_INT_RT);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 4);

	while(UARTCharsAvail(UART1_BASE))
	{
		BUF[BUFindex++]=UARTCharGet(UART1_BASE);
	}






}

void UART0IntHandler(void)                         //接收中断
{
	char R;
	UARTIntClear(UART0_BASE,UART_INT_RX|UART_INT_RT);
	if(UARTCharsAvail(UART0_BASE))
	R=UARTCharGet(UART0_BASE);

	if(R=='!')
	{

			//GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 8);

			buttonflag=1;

	}
}
