#include<LPC21XX.h>
#include<string.h>
#include"delay.h"
#include"lcd4bit.h"
#include"keypad.h"
#include"i2c.h"
#include"uart.h"
#include<stdio.h>
#include<stdlib.h>
//#include<unistd>

#define sw1 14
#define mot1 1<<12
#define mot2 1<<13

//u8 p[9] __attribute__((at(0x40000040)))="";
 u8 p[9];
 char mssg[50];

/*int generateOTP(char* otp,int len)
{
	unsigned int seed=0;
	unsigned int timer_read;
	int i;

	for(i=0;i<len;i++)
	{
		T0TCR=1;//start timer
		delay_ms(5+i);//variable delay to mix randomness
		T0TCR=0;//stop timer
		timer_read=T0TC;//read timer counter
		seed^=timer_read;//xor timer for more randomness
		seed^=(IOPIN0& 0xff);//mix  random GPIO pins
	}
	srand(seed);//seed the random number
	//generate otp
	for(i=0;i<len;i++)
	{
	otp[i]='0'+(rand() % 10); //random digits between 0-9
	}
	otp[len]='\0';// null terminate the string
	//x[len]=otp;
	return otp[5];

}  

int generateOTP(char* otp,int len)
{
	int i;
	srand((unsigned long int)&i);
	srand(getpid());
	for(i=0;i<5;i++)
	a[i]=rand()%(mmax-min)+min;
	for(i=0;i<5;i++)
	otp[5]=a[i];		


}
	  */


int main()
{
	
	int n;
	int byte;
	int sms,count;
	//int txt;
	
	char a[20];
	char b[20];

	//char otp[5]; //4 digits OTP+'\0'
	int otp=5216;
	
	lcd_config();
	UART0_config();
	init_i2c();
	 
	
//	PINSEL0|=mot1 | mot2;
	IODIR0|=mot1 | mot2;
//	  IODIR0|=mot1;

	lcd_command(0x80);
	//lcd_scroll("MULTILEVEL SECURITY BASED ACCESS CONTROL SYSTEM");
	//lcd_string("MULTILEVEL SECURITY BASED ACCESS CONTROL SYSTEM");
	lcd_scroll("MULTILEVEL SECURITY");
	delay_ms(300);
	lcd_command(0x01);
	
	i2c_eeprom_page_write(0x50,0x00,"1234",4);
    i2c_eeprom_seq_read(0x50,0x00,p,4);
	delay_ms(150); 
 	while(1)
	{	  
	if((IOPIN0>>sw1&1)==0)
	{
	delay_ms(200);//switch debouncing
	
  pass:  
  	lcd_command(0x01);
	lcd_command(0x80);
	lcd_string("Enter Password");
	lcd_command(0xc0);
	for(n=0;n<=3;n++)
	{
	byte=keyscan()+48;
	lcd_data('*');
	a[n]=byte;
	}
	a[n]='\0';
	if(strcmp(p,a)==0)
{
	lcd_command(0x01);  	
	lcd_command(0x80);
	lcd_string("Password Matched");
	delay_ms(500);
	lcd_command(0x01);
	lcd_command(0x80);
	lcd_string("Generating OTP");
	delay_ms(500);

	UART0_config();
	lcd_command(0x01);
	lcd_string("Otp sending..");
	
	
	UART0_string("AT\r\n");
	delay_ms(1000);
	UART0_string("AT+CMGF=1\r\n");
	delay_ms(1000);
	UART0_string("AT+CMGS=\"+918870834233\"\r\n");
	delay_ms(1000);
	
//	generateOTP(otp,5);//Generate 4 digit otp
	
	sprintf(mssg,"Your OTP is %d",otp);
	UART0_string(mssg);
	UART0_string("\r\n");
	delay_ms(1000);
	UART0_TX(0x1A);
	delay_ms(1000);

	lcd_command(0x01);
 	lcd_command(0x80);
 	lcd_string("Enter OTP");
	lcd_command(0xc0);	
	n=0;
	while(n<=3)
	{
	b[n]=keyscan()+48;
	lcd_data(b[n]);
	n++;										 
  	}
    //sms=b[n];
	sms=atoi(b);
//txt=atoi(otp);

	if(sms==otp)
	{
		lcd_command(0x01);
	  	lcd_command(0x80);
	  	lcd_string("OTP matched");
		delay_ms(500);
		lcd_command(0x01);
	  	lcd_command(0x80);
	  	lcd_string("DOOR UNLOCKED");
		
		IOSET0=mot1;
		IOCLR0=mot2;
		delay_sec(5);
        IOCLR0=mot1;
		IOCLR0=mot1 | mot2;
		delay_ms(500);
		IOCLR0=mot1;
		IOSET0=mot2;
		delay_sec(5);
		IOCLR0=mot1 | mot2;
		
		}

	}
  else
		{	
		lcd_command(0x01);
	  	lcd_command(0x80);
	  	lcd_string("Wrong Password");
		delay_ms(300);
		count++;
		if(count==3)
		{
			lcd_command(0x01);
			lcd_command(0x80);
			lcd_string("someone accessing the door");
			UART0_string("AT\r\n");
			delay_ms(500);
			UART0_string("AT+CMGF=1\r\n");
			delay_ms(500);
			UART0_string("AT+CMGS=\"+918870834233\"\r\n");
			delay_ms(500);
			UART0_string("ALERT:Someone is accesssing the door\r\n");
			delay_ms(500);
			UART0_TX(0x1A);
			return 0;
		}
		goto pass;		
	}

											  
	} 

}
} 

