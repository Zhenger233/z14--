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

int weishu(long n)//返回n的位数
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
  if(test==1)n=65534;//test=1时测试扩展功能显示65535
  else if(test==-1)n=-65534;//test=-1时测试扩展功能显示-65535
  else n=0;
  
  P1DIR |= BIT0;//设定为输出
  P4DIR |=BIT7;
  WDTCTL = WDTPW+WDTHOLD;  // Stop WDT
  ClkInit();               //时钟初始化
  KeyInit();               //独立按键初始化
  LCD_Init();              //显示屏初始化
  
  LCD_ShowNum(0,0,2019,4,16); //初始化显示为2019
  
  P1OUT &=~ BIT0;//P1.0灭
  P4OUT|=BIT7;//P4.7亮
  
  while(1)
  {
    key = KeyScan();//按键检测
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
      wn=weishu(n);//wn为n的位数
      
     if(clsflag==1)//消去初始化的2019
      {
        LCD_CLS();
        clsflag=0;
      }
     
      nn=n>=0?n:-n;//防止n小于0时无法取余
      if(nn%2==1)//交叉亮灯
      {
        P1OUT |= BIT0;
        P4OUT &=~ BIT7;
      }
      else
      {
        P1OUT &=~ BIT0;
        P4OUT |= BIT7;
      }
      
      widenew=wn;//显示总宽度改变时清屏，防止遗留后置位
      if(n<0)widenew++;
      if(widenew!=wideold)LCD_CLS();
      wideold=widenew;
      
      if(n<-65535)//扩展要求
      {
        LCD_ShowChar(0,0,'-',16);
        LCD_ShowNum(9,0,65535,5,16);//一个字符占8个宽度
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