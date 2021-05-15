#include "display.h"
#include "oled.h"

void Display_Normal(int value_1,int value_2,int value_3,int value_4)
{
	OLED_ShowNum(80,4,value_1,2,16);
	OLED_ShowChar(96,4,'.',16);
	OLED_ShowNum(104,4,value_2,1,16);
	OLED_ShowChar(112,4,'C',16);
	OLED_ShowNum(80,6,value_3,2,16);
	OLED_ShowChar(96,6,'.',16);
	OLED_ShowNum(104,6,value_4,1,16);
	OLED_ShowChar(112,6,'%',16);
	
	HAL_Delay(1000);
}
