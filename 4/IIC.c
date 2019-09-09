#include        "IIC.h"
#include        "font.h"

/************************************************************
  �ļ����ƣ�                IIC.c
  �ļ�������                ͨ������IICͨ�Žӿڶ�oled���п���
  ������ͷ�ļ���������      IIC.h ���������������ֺ궨��
                            font.h�������ֿ� �����ַ�����������
  ����������                
                            IICͨ��ʱ���ߣ�P6.0
                            IICͨ�������ߣ�P6.5
*************************************************************/  



/************************************************************/
//������     ��IIC_delayus
//��������ֵ ��i����ʱ���Ʊ���
//��������ֵ ����
//��������   ����ʱ1us
//ע������   ��������4Mʱ�ӵ�ǰ���½��еķ�Ƶ��������������ʱ�������仯���ú���ҲҪ������Ӧ�ı仯
void IIC_delayus(unsigned int i)
{
  unsigned int j,k;
  for(k=0;k<i;k++)
    for(j=0;j<4;i++);
}
/************************************************************/



/************************************************************/
//������     ��IIC_ioinit
//��������ֵ ����
//��������ֵ ����
//��������   ����IICͨ��ʹ�õ�I/O�ڽ�������
void IIC_ioinit()
{
  P6DIR|=BIT0|BIT5; //����IIC IO��Ϊ�������
  P6REN|=BIT0|BIT5; //����IIC IO��ʹ����������
}
/************************************************************/




/************************************************************/
//������     ��IIC_start
//��������ֵ ����
//��������ֵ ����
//��������   ����ʼIICͨ�ŵĲ���
void IIC_start()
{
   SCL_HIGH;
   SDA_HIGH;
   //IIC_delayus(2); //*�˴���������Ƿ�������ʱ
   SDA_LOW;
   //IIC_delayus(2);
   SCL_LOW;
}
/************************************************************/




/************************************************************/
//������     ��IIC_stop
//��������ֵ ����
//��������ֵ ����
//��������   ��ֹͣIICͨ�ŵĲ���
void IIC_stop()
{
     SCL_LOW;
     SDA_LOW;
     //IIC_delayus(2);
     SCL_HIGH;
     SDA_HIGH;
     //IIC_delayus(2);
}
/************************************************************/



/************************************************************/
//������     ��IIC_writebyte
//��������ֵ ��IIC_byte����Ҫ���͵��ֽ�����
//��������ֵ ����
//��������   ����IIC���߷���һ���ֽ�
void IIC_writebyte(unsigned char IIC_byte)
{
   unsigned char i;
   for(i=0;i<8;i++)  
   {
      if(IIC_byte&0x80)  
        SDA_HIGH;
      else
        SDA_LOW;
      SCL_HIGH;
      SCL_LOW;
      IIC_byte<<=1;   //loop
   }
   SDA_HIGH;
   SCL_HIGH;
   SCL_LOW;
}
/************************************************************/




/************************************************************/
//������     ��IIC_writecmd
//��������ֵ ��IIC_command����Ҫ���͵�ָ������
//��������ֵ ����
//��������   ����IIC���߷���һ��ָ��
void IIC_writecmd(unsigned char IIC_command)
{
   IIC_start();
   IIC_writebyte(0x78);   //Slave address,SA0=0
   IIC_writebyte(0x00);   //write command
   IIC_writebyte(IIC_command);
   IIC_stop();
}
/************************************************************/



/************************************************************/
//������     ��IIC_writedata
//��������ֵ ��IIC_data����Ҫ���͵���������
//��������ֵ ����
//��������   ����IIC���߷���һ������
void IIC_writedata(unsigned char IIC_data)
{
    IIC_start();
    IIC_writebyte(0x78);   
    IIC_writebyte(0x40);   //write data
    IIC_writebyte(IIC_data);
    IIC_stop();
}
/************************************************************/



// ˵�����������ĺ�����ͨ��IIC������oled���п��Ƶĺ�����ֱ�Ӹ��ݹ��ܵ��ü���
// �����б�����Ӧ���ܣ�
// 1��LCD_Set_Pos   ������oled������
// 2��LCD_CLS       ����oled��������
// 3��LCD_Init      ����oled���г�ʼ������
// 4��LCD_ShowChar  ����ָ�����괦��ʾһ���ַ��������ӿ����·���д��
// 5��LCD_ShowString����ָ�����괦��ʾһ���ַ���
// 6��LCD_ShowNum   ����ָ�����괦��ʾһ������




/**************************oled��������*******************************/
void LCD_Set_Pos(unsigned char x, unsigned char y)
{
    IIC_writecmd(0xb0+y);
    IIC_writecmd(((x&0xf0)>>4)|0x10);
    IIC_writecmd((x&0x0f)|0x01);
}


/***************************oled����************************************/
void LCD_CLS(void)
{
  unsigned char y,x;        
  for(y=0;y<8;y++)
  {
    IIC_writecmd(0xb0+y);
    IIC_writecmd(0x01);
    IIC_writecmd(0x10);
    for(x=0;x<X_WIDTH;x++)
    IIC_writedata(0);
   }
}


/***************************oled��ʼ��************************************/
void LCD_Init(void)
{
  IIC_ioinit();
  
  
  
  IIC_writecmd(0xAE);   //display off
  IIC_writecmd(0x20); //Set Memory Addressing Mode
  IIC_writecmd(0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
  IIC_writecmd(0xb0); //Set Page Start Address for Page Addressing Mode,0-7
  IIC_writecmd(0xc8); //Set COM Output Scan Direction
  IIC_writecmd(0x00);//---set low column address
  IIC_writecmd(0x10);//---set high column address
  IIC_writecmd(0x40);//--set start line address
  IIC_writecmd(0x81);//--set contrast control register
  IIC_writecmd(0x7f);
  IIC_writecmd(0xa1);//--set segment re-map 0 to 127
  IIC_writecmd(0xa6);//--set normal display
  IIC_writecmd(0xa8);//--set multiplex ratio(1 to 64)
  IIC_writecmd(0x3F);//
  IIC_writecmd(0xa4);//0xa4,Output follows RAM content;0xa5,Output ignores RAM content
  IIC_writecmd(0xd3);//-set display offset
  IIC_writecmd(0x00);//-not offset
  IIC_writecmd(0xd5);//--set display clock divide ratio/oscillator frequency
  IIC_writecmd(0xf0);//--set divide ratio
  IIC_writecmd(0xd9);//--set pre-charge period
  IIC_writecmd(0x22); //
  IIC_writecmd(0xda);//--set com pins hardware configuration
  IIC_writecmd(0x12);
  IIC_writecmd(0xdb);//--set vcomh
  IIC_writecmd(0x20);//0x20,0.77xVcc
  IIC_writecmd(0x8d);//--set DC-DC enable
  IIC_writecmd(0x14);//
  IIC_writecmd(0xaf);//--turn on oled panel
  LCD_CLS();
  LCD_Set_Pos(0,0);
}


//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 16/12 
void LCD_ShowChar(unsigned char x,unsigned char y,unsigned char chr,unsigned char Char_Size)
{      	
  unsigned char c=0,i=0;	
  c=chr-' ';//�õ�ƫ�ƺ��ֵ			
  if(x>Max_Column-1)
  {
     x=0;
     y=y+2;
  }
  if(Char_Size ==16)
  {
     LCD_Set_Pos(x,y);	
     for(i=0;i<8;i++)
     IIC_writedata(F8X16[c*16+i]);
     LCD_Set_Pos(x,y+1);
     for(i=0;i<8;i++)
     IIC_writedata(F8X16[c*16+i+8]);
   }
   else 
   {	
     LCD_Set_Pos(x,y);
     for(i=0;i<6;i++)
     IIC_writedata(F6x8[c][i]);			
   }
}


//��ʾһ���ַ��Ŵ�
void LCD_ShowString(unsigned char x,unsigned char y,unsigned char *chr,unsigned char Char_Size)
{
   unsigned char j=0;
   while (chr[j]!='\0')
   {		
     LCD_ShowChar(x,y,chr[j],Char_Size);	
     x+=8;
     if(x>128)
     {
       x=0;
       y+=2;
     }
     j++;	
   }
}



//m^n����
unsigned long oled_pow(unsigned char m,unsigned char n)
{
	unsigned long result=1;	 
	while(n--)result*=m;    
	return result;
}	



//��ʾһ������
void LCD_ShowNum(unsigned char x,unsigned char y,unsigned long num,unsigned char len,unsigned char size2)
{         	
	unsigned char t,temp;
	unsigned char enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
}