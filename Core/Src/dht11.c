#include "dht11.h"
#include "main.h"
#include "tim.h"

void DHT11_IO_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure = {0};
	GPIO_InitStructure.Pin = DHT11_Value_Pin;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(DHT11_Value_GPIO_Port,&GPIO_InitStructure);
}

void DHT11_IO_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure = {0};
	GPIO_InitStructure.Pin = DHT11_Value_Pin;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(DHT11_Value_GPIO_Port,&GPIO_InitStructure);
}

//��λDHT11
void DHT11_Rst(void)	   
{                 
	DHT11_IO_OUT();//����Ϊ���
	DHT11_DQ_OUT_LOW;//����DQ
	HAL_Delay(20);//��������18ms
	DHT11_DQ_OUT_HIGH;//DQ=1 
	HAL_Delay_us(30);//��������20~40us
}

//�ȴ�DHT11�Ļ�Ӧ
//����1:δ��⵽DHT11�Ĵ���
//����0:����
uint8_t DHT11_Check(void) 	   
{   
	uint8_t retry=0;
	DHT11_IO_IN();//����Ϊ���	 
	while (DHT11_DQ_IN&&retry<100)//DHT11������40~80us
	{
		retry++;
		HAL_Delay_us(1);
	};	 
	if(retry>=100)
	{
		return 1;
	}
	else
	{
		retry=0;
	}
	while(!DHT11_DQ_IN&&retry<100)//DHT11���ͺ���ٴ�����40~80us
	{
		retry++;
		HAL_Delay_us(1);
	};
	if(retry>=100)
	{
		return 1;	    
	}
	return 0;
}

//��DHT11��ȡһ��λ
//����ֵ��1/0
uint8_t DHT11_Read_Bit(void) 			 
{
 	uint8_t retry=0;
	while(DHT11_DQ_IN&&retry<100)//�ȴ���Ϊ�͵�ƽ
	{
		retry++;
		HAL_Delay_us(1);
	}
	retry=0;
	while(!DHT11_DQ_IN&&retry<100)//�ȴ���ߵ�ƽ
	{
		retry++;
		HAL_Delay_us(1);
	}
	HAL_Delay_us(40);//�ȴ�40us
	if(DHT11_DQ_IN)
	{
		return 1;
	}
	else
	{
		return 0;		
	}		
}

//��DHT11��ȡһ���ֽ�
//����ֵ������������
uint8_t DHT11_Read_Byte(void)    
{        
	uint8_t i,dat;
	dat=0;
	for (i=0;i<8;i++) 
	{
		dat<<=1; 
	  dat|=DHT11_Read_Bit();
  }						    
	return dat;
}

//��DHT11��ȡһ������
//temp:�¶�ֵ(��Χ:0~50��)
//humi:ʪ��ֵ(��Χ:20%~90%)
//����ֵ��0,����;1,��ȡʧ��
uint8_t DHT11_Read_Data(uint16_t *temp,uint16_t *humi)    
{        
 	uint8_t buf[5];
	uint8_t i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//��ȡ40λ����
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humi=(buf[0]<<8) + buf[1];
			*temp=(buf[2]<<8) + buf[3];
		}
	}
	else
	{
		return 1;
	}
	return 0;	    
}

//��ʼ��DHT11��IO�� DQ ͬʱ���DHT11�Ĵ���
//����1:������
//����0:����     	 
uint8_t DHT11_Init(void)
{ 
  DHT11_Rst();
	return DHT11_Check();
}
