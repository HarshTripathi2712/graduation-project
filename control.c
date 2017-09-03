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


#define Second 4000000
#define MilliSecond 4000
#define MicroSecond 4
unsigned long TheSysClock=12000000UL;

#define GPIO_PA0_U0RX           0x00000001
#define GPIO_PA1_U0TX           0x00000401
#define GPIO_PB0_U1RX           0x00010001
#define GPIO_PB1_U1TX           0x00010401


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
//in3in4-->右轮10向后01向前
//in1in2-->左轮10向后01向前
void stop(void)
{
	//EN1-->PC6
		HWREGBITB(0x400243fc,2)=1;
		//EN2-->PD0
		HWREGBITB(0x400073fc,0)=1;
		//IN1-->PD1
		HWREGBITB(0x400073fc,1)=0;
		//IN2-->PC4
		HWREGBITB(0x400073fc,3)=0;
		//IN3-->PC5
		HWREGBITB(0x400243fc,1)=0;
		//IN4-->PD2
		HWREGBITB(0x400073fc,2)=0;
}
void front(void)
{
	//EN1-->PE2
	HWREGBITB(0x400243fc,2)=1;
	//EN2-->PD0
	HWREGBITB(0x400073fc,0)=1;
	//IN1-->PD1
	HWREGBITB(0x400073fc,1)=0;
	//IN2-->PD3
	HWREGBITB(0x400073fc,3)=1;
	//IN3-->PE1
	HWREGBITB(0x400243fc,1)=0;
	//IN4-->PD2
	HWREGBITB(0x400073fc,2)=1;




}
void back(void)  //111010
{
	//EN1-->PE2
		HWREGBITB(0x400243fc,2)=1;
		//EN2-->PD0
		HWREGBITB(0x400073fc,0)=1;
		//IN1-->PD1
		HWREGBITB(0x400073fc,1)=1;
		//IN2-->PD3
		HWREGBITB(0x400073fc,3)=0;
		//IN3-->PE1
		HWREGBITB(0x400243fc,1)=1;
		//IN4-->PD2
		HWREGBITB(0x400073fc,2)=0;
}
void left(void)
{
	//EN1-->PE2
		HWREGBITB(0x400243fc,2)=1;
		//EN2-->PD0
		HWREGBITB(0x400073fc,0)=1;
		//IN1-->PD1
		HWREGBITB(0x400073fc,1)=0;
		//IN2-->PD3
		HWREGBITB(0x400073fc,3)=0;
		//IN3-->PE1
		HWREGBITB(0x400243fc,1)=1;
		//IN4-->PD2
		HWREGBITB(0x400073fc,2)=0;
}
void right(void)
{
	//EN1-->PE2
		HWREGBITB(0x400243fc,2)=1;
		//EN2-->PD0
		HWREGBITB(0x400073fc,0)=1;
		//IN1-->PD1
		HWREGBITB(0x400073fc,1)=0;
		//IN2-->PD3
		HWREGBITB(0x400073fc,3)=1;
		//IN3-->PE1
		HWREGBITB(0x400243fc,1)=0;
		//IN4-->PD2
		HWREGBITB(0x400073fc,2)=0;
}
void stay_left(void)  //111100
{
	//EN1-->PE2
		HWREGBITB(0x400243fc,2)=1;
		//EN2-->PD0
		HWREGBITB(0x400073fc,0)=1;
		//IN1-->PD1
		HWREGBITB(0x400073fc,1)=1;
		//IN2-->PD3
		HWREGBITB(0x400073fc,3)=0;
		//IN3-->PE1
		HWREGBITB(0x400243fc,1)=0;
		//IN4-->PD2
		HWREGBITB(0x400073fc,2)=1;
}
void stay_right(void)
{
	//EN1-->PC6
		HWREGBITB(0x400243fc,2)=1;
		//EN2-->PD0
		HWREGBITB(0x400073fc,0)=1;
		//IN1-->PD1
		HWREGBITB(0x400073fc,1)=0;
		//IN2-->PC4
		HWREGBITB(0x400073fc,3)=1;
		//IN3-->PC5
		HWREGBITB(0x400243fc,1)=1;
		//IN4-->PD2
		HWREGBITB(0x400073fc,2)=0;
}

void UARTStringGet()    //蓝牙传输的是16进制输不是字符
{

	char a;
	a=UARTCharGet(UART1_BASE);

		if(a==1)
		{
			front();
		}
		if(a==2)
		{
			left();
		}
		if(a==3)
		{
			stop();
		}
		if(a==4)
		{
			right();
		}
		if(a==5)
		{
			back();
		}
		if(a==6)
		{
			stay_left();
		}
		if(a==7)
		{
			stay_right();
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





	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	GPIOPinConfigure(GPIO_PB0_U1RX);
	GPIOPinConfigure(GPIO_PB1_U1TX);
	GPIOPinTypeUART(GPIO_PORTB_BASE,GPIO_PIN_0|GPIO_PIN_1);
	UARTConfigSetExpClk(UART1_BASE,SysCtlClockGet(),9600,(UART_CONFIG_WLEN_8|UART_CONFIG_STOP_ONE|UART_CONFIG_PAR_NONE));

	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE,GPIO_PIN_0|GPIO_PIN_1);
	UARTConfigSetExpClk(UART0_BASE,SysCtlClockGet(),9600,(UART_CONFIG_WLEN_8|UART_CONFIG_STOP_ONE|UART_CONFIG_PAR_NONE));



	IntEnable(INT_UART1);

	UARTIntEnable(UART1_BASE,UART_INT_RX|UART_INT_RT);

	IntMasterEnable();


	UARTEnable(UART1_BASE);
	UARTCharPut(UART1_BASE,'!');
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 4);
	//front();
	//left();
	//back();
	//right();
	while(1)
	{

	}
}


void UART1IntHandler(void)
{

	UARTIntClear(UART1_BASE,UART_INT_RX|UART_INT_RT);

	UARTStringGet();

	//GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 4);




}



