#include<string.h>

void UART0_config(void);
void pin_config(void);
void UART0_data(unsigned char);
unsigned char UART0_RX(void);
void UART0_string(unsigned char*);
unsigned char arr[20];
int i;
unsigned char rx_byte;
void UART0_config(void)
{
PINSEL0|=0X00000005;
U0LCR=0X83;
U0DLL=97;
U0DLM=0;
U0LCR=0X03;
}   
     
void UART0_TX(unsigned char d)
{
U0THR=d;
while(((U0LSR>>5)&1)==0);
}
unsigned char UART0_RX(void)
{
while((U0LSR&1)==0);
return U0RBR;
}
void UART0_string(unsigned char*s)
{
while(*s)
UART0_TX(*s++);
}




					