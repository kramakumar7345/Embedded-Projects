 
#define SCL_EN  0x00000010

#define SDA_EN  0x00000040

#define CCLK      60000000  //Hz

#define PCLK      CCLK/4    //Hz

#define I2C_SPEED 100000    //Hz

#define LOADVAL   ((PCLK/I2C_SPEED)/2)


#define   AA_BIT 2

#define   SI_BIT 3

#define  STO_BIT 4

#define  STA_BIT 5

#define I2EN_BIT 6

typedef unsigned char u8;
typedef unsigned int u32;


void init_i2c(void)

{	

	PINSEL0 |=SCL_EN|SDA_EN;

	I2SCLL=LOADVAL;

    I2SCLH=LOADVAL;

	I2CONSET=1<<I2EN_BIT; 

}

void i2c_start(void)

{
	I2CONSET=1<<STA_BIT;

	while(((I2CONSET>>SI_BIT)&1)==0);

	I2CONCLR=1<<STA_BIT;

}	


void i2c_restart(void)

{

	I2CONSET=1<<STA_BIT;

	I2CONCLR=1<<SI_BIT;

	while(((I2CONSET>>SI_BIT)&1)==0);

	I2CONCLR=1<<STA_BIT;

}	


void i2c_write(u8 dat)

{

	I2DAT=dat;	

	I2CONCLR = 1<<SI_BIT;

	while(((I2CONSET>>SI_BIT)&1)==0);

}	


void i2c_stop(void)

{

	I2CONSET=1<<STO_BIT;

  	I2CONCLR = 1<<SI_BIT;

}


u8 i2c_nack(void)

{	

	I2CONSET = 0x00; //Assert Not of Ack

  	I2CONCLR = 1<<SI_BIT;

	while(((I2CONSET>>SI_BIT)&1)==0);

	return I2DAT;

}


u8 i2c_masterack(void)

{	

	I2CONSET = 0x04; //Assert Ack

  	I2CONCLR = 1<<SI_BIT;

	while(((I2CONSET>>SI_BIT)&1)==0);

	I2CONCLR = 0x04; //Clear Assert Ack

 	return I2DAT;

}	

void i2c_eeprom_page_write(u8 slaveAddr,u8 wBuffStartAddr,u8 *p,u8 nBytes)

{

  u8 i;

  i2c_start();	

  i2c_write(slaveAddr<<1);    //slaveAddr + w

	i2c_write(wBuffStartAddr);  //wBuffStartAddr

	for(i=0;i<nBytes;i++)

	{

	   i2c_write(p[i]);             //wBuffAddr

	}

	i2c_stop();

	delay_ms(10);

}			 


void i2c_eeprom_seq_read(u8 slaveAddr,u8 rBuffStartAddr,u8 *p,u8 nBytes)
{

	u8 i;

	i2c_start();	

  i2c_write(slaveAddr<<1); //slaveAddr + w

	i2c_write(rBuffStartAddr);    //rBuffAddr

	i2c_restart();	

	i2c_write(slaveAddr<<1|1); //slaveAddr + r

	for(i=0;i<nBytes-1;i++)

	{

    p[i]=i2c_masterack();	

	}

	p[i]=i2c_nack();

	i2c_stop();

}



