
void delay_ms(unsigned int ms)
{
T0PR=15000-1;
T0TCR=0X1;
while(T0TC<ms);
T0TCR=0X3;
T0TCR=0X0;
}

void delay_sec(unsigned int sec)
{
T0PR=15000000-1;
T0TCR=0X1;
while(T0TC<sec);
T0TCR=0X3;
T0TCR=0X0;
}