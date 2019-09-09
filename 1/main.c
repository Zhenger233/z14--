/*
LeftKey  2.1
LeftLed  1.0
RightKey 1.1
RightLed 4.7
*/
#include "MSP430F5529.h"
#include "clk.c"
#include "key.c"
#include "timer.c"
#include "IIC.c"

void delay(int t)
{
    int a=1200;
    for(;t>=0;--t)for(;a>=0;--a);
}

int weishu(long n)//����n��λ��
{
  if(n==0)return 1;
  int i=0;
  while(n)
  {
    i++;
    n/=10;
  }
  return i;
}

int main( void )
{
  enum key_type key;
  int 
    clsflag=1,test=0,wideold=4,widenew=4,wn;
  long n,nn;
  if(test==1)n=65534;//test=1ʱ������չ������ʾ65535
  else if(test==-1)n=-65534;//test=-1ʱ������չ������ʾ-65535
  else n=0;
  
  P1DIR |= BIT0;//�趨Ϊ���
  P4DIR |=BIT7;
  WDTCTL = WDTPW+WDTHOLD;  // Stop WDT
  ClkInit();               //ʱ�ӳ�ʼ��
  KeyInit();               //����������ʼ��
  LCD_Init();              //��ʾ����ʼ��
  
  LCD_ShowNum(0,0,2019,4,16); //��ʼ����ʾΪ2019
  
  P1OUT &=~ BIT0;//P1.0��
  P4OUT|=BIT7;//P4.7��
  
  while(1)
  {
    key = KeyScan();//�������
    if( key != NOKEY )
    {
      switch(key)
      {
        case KEYLEFT  : 
          {
            n--;
            break;
          }      
        case KEYRIGHT :   
          {
            n++;
          }
      }
      wn=weishu(n);//wnΪn��λ��
      
     if(clsflag==1)//��ȥ��ʼ����2019
      {
        LCD_CLS();
        clsflag=0;
      }
     
      nn=n>=0?n:-n;//��ֹnС��0ʱ�޷�ȡ��
      if(nn%2==1)//��������
      {
        P1OUT |= BIT0;
        P4OUT &=~ BIT7;
      }
      else
      {
        P1OUT &=~ BIT0;
        P4OUT |= BIT7;
      }
      
      widenew=wn;//��ʾ�ܿ��ȸı�ʱ��������ֹ��������λ
      if(n<0)widenew++;
      if(widenew!=wideold)LCD_CLS();
      wideold=widenew;
      
      if(n<-65535)//��չҪ��
      {
        LCD_ShowChar(0,0,'-',16);
        LCD_ShowNum(9,0,65535,5,16);//һ���ַ�ռ8������
      }
      else if(n<0)
      {
        LCD_ShowChar(0,0,'-',16);
        LCD_ShowNum(9,0,-n,wn,16);
      }
      else if(n>65535)LCD_ShowNum(0,0,65535,5,16);
      else if(n>0)LCD_ShowNum(0,0,n,wn,16);
      else LCD_ShowNum(0,0,0,1,16);
    }
    delay(1000);
  }
}