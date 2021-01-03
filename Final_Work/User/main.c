/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   TIM timer
  ******************************************************************************
  * @attention
	*  屏幕动态显示数字
  *  sprintf(str_temperature, "temperature:%d", temperature);
	*	 sprintf(str_humidity, "humidity:%d", humidity);
  *
  ******************************************************************************
  */ 
		
#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_BasicTim.h"
#include "delay.h"
#include "sys.h"
#include "beep.h"
#include "bsp_key.h"
#include "tftlcd.h"
#include "bsp_dht11.h"
#include "bsp_usart.h"
#include "string.h"
#include "vl53l0x.h"
#include "common.h"
#include "bsp_esp8266.h"
#include "test.h"
#include "hcsr04.h"


/************************************************************************/
//                        DHT11全局变量                                                       
/************************************************************************/
uint8_t t = 0; //用来控制读取dht11时间间隔
uint8_t temperature = 0;
uint8_t humidity = 0;
char str_temperature[16] = "";
char str_humidity[16] = "";
char string[16] = {0};


/************************************************************************/
//                        激光测距vl5310x全局变量                                                       
/************************************************************************/

extern VL53L0X_Dev_t vl53l0x_dev;//设备I2C数据参数
//u16 dist[32] = {0};   //滑动均值滤波
u16 dist = 0;
u8  dist_cnt = 0;

extern vu16 Distance_data;
int ROOM_STATUS[4] = {0, 1, 2, 3};
int room_status = 0;

char str_distance_1_busy[36] = {0};
char str_distance_1_available[36] = {0};
char str_room_1_time[36] = {0};
char str_room_2_time[36] = {0};

void distance_deal(VL53L0X_Error status);
VL53L0X_Error Status;

int show_time_1 = 0;
int show_time_2 = 0;
int time_1_counter = 0;
int time_2_counter = 0;
int LED_1_counter = 0;
int LED_2_counter = 0;
int LED_RED_Signal = 0;

//动态更新一次所需时间，单位ms
int unit_time = 1000;
//红灯闪烁间隔，单位ms
int unit_LED = 200;

int User_Fine = 0;

int laser_safe_distance = 150;

/************************************************************************/
//                        ESP8266全局变量                                                       
/************************************************************************/
char cStr [ 100 ] = { 0 };
char WIFI_Information[100] = {0};



/************************************************************************/
//                        HC-SR04全局变量                                                       
/************************************************************************/
int echo_time = 0;
uint16_t UltraWave_Distance = 0;
int UltraWave_safe_distance = 25;
uint32_t BEEP_Counter = 0;
int BEEP_Interval = 100;

/**
  * @brief  
  * @param   
  * @retval 
  */
int main(void)
{
	/*************变量定义******************/
	DHT11_Data_TypeDef DHT11_Data;
	
	/*变量定义END*/
	
	
	
	/* 初始化 */
	delay_init();
	LED_GPIO_Config();
	USART_Config();
	BEEP_GPIO_Config();
	EXTI_Key_Config();
	
	LCD_Init();	
	DHT11_Init();
	ESP8266_Init ();
	HCSR04_Init();
	TIM_Cmd(Echo_TIM, DISABLE);
	
	TIM_Init();
	TIM_Cmd(BASIC_TIM_1_time, DISABLE);
	TIM_Cmd(BASIC_TIM_1_LED, DISABLE);
	//TIM_Cmd(BEEP_TIM, DISABLE);
	
	VL53L0X_i2c_init();
	if(vl53l0x_set_mode(&vl53l0x_dev,0))   //配置测量模式 0:default   1:High Accuracy  2:Long Range   3:High Speed
	{
//			printf("Mode Set Error");
		LED_PURPLE;
	}
	else
	{
//		printf("VL53L0X_Init OK");
	}
	
	/*初始化END*/
	
	
	
	/*配置LCD　START*/
	
	LCD_Clear(WHITE);
	
	//写字流程，先设置画笔和背景颜色，再指定位置和字符
	POINT_COLOR = WHITE;
	BACK_COLOR = BLUE;
	LCD_ShowString(0, 0, 480, 48, 32, "  SCU  Toilet   "); //480是屏幕总宽
	
	//设置数据的显示样式
	POINT_COLOR = BLACK;
	BACK_COLOR = WHITE;
	
	/*配置LCD　END*/
	
	
	
	/************************************************************************/
	/*                        配置ESP8266 START                               */
	/************************************************************************/
	
	POINT_COLOR = BRRED;

	LCD_ShowString(0, 168, 480, 16, 16, "ESP8266 START");
	
	macESP8266_CH_ENABLE();
	
	ESP8266_AT_Test ();

	
	ESP8266_Net_Mode_Choose ( AP );
	
	// 设置AP模式的IP地址
	while ( ! ESP8266_CIPAP ( macUser_ESP8266_TcpServer_IP ) );

	
	// 设置AP模式的 WIFI名称、密码、加密方式
	while ( ! ESP8266_BuildAP ( macUser_ESP8266_BulitApSsid, macUser_ESP8266_BulitApPwd, macUser_ESP8266_BulitApEcn ) );

	
	// 启动多连接（最多连5个）
	ESP8266_Enable_MultipleId ( ENABLE );
	

	
	// 开启服务器模式，即设置端口号，与超时时间
	while ( !	ESP8266_StartOrShutServer ( ENABLE, macUser_ESP8266_TcpServer_Port, macUser_ESP8266_TcpServer_OverTime ) );
	
	
	ESP8266_Inquire_ApIp ( cStr, 20 );
	
	sprintf(WIFI_Information, "WIFI:%s", macUser_ESP8266_BulitApSsid);
	LCD_ShowString(0, 168, 480, 16, 16, WIFI_Information);
	
	sprintf(WIFI_Information, "IP:%s", cStr);
	LCD_ShowString(0, 188, 480, 16, 16, WIFI_Information);
	
	sprintf(WIFI_Information, "Port:%s", macUser_ESP8266_TcpServer_Port);
	LCD_ShowString(0, 208, 480, 16, 16, WIFI_Information);
	
	strEsp8266_Fram_Record .InfBit .FramLength = 0;
	strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;
	
	POINT_COLOR = BLACK;
	
	/************************************************************************/
	/*                        配置ESP8266 END                               */
	/************************************************************************/
	
	
	
	
	
	
	
	
	
	
	
	#if 1
	while(1)
	{
		
		/*DHT11 START*/
		if( DHT11_Read_TempAndHumidity ( & DHT11_Data ) == SUCCESS)
    {
		  sprintf(str_temperature, "temperature:%d.%d", DHT11_Data.temp_int,DHT11_Data.temp_deci);
			LCD_ShowString(0, 36, 480, 24, 24, str_temperature);	//显示温度
			
		  sprintf(str_humidity, "humidity:%d.%d", DHT11_Data.humi_int,DHT11_Data.humi_deci);
		  LCD_ShowString(0, 64, 480, 24, 24, str_humidity);   //显示湿度
		 
    }
	  /*DHT11 END*/
		
		
		/************************************************************************/
		/*                        超声波测距 START                               */
		/************************************************************************/
		
		UltraWave_Distance = Hcsr04_Get_Distance();
		//printf("distance:%d", UltraWave_Distance);
		//delay_ms(100);
		
		/************************************************************************/
		/*                        超声波测距 END                               */
		/************************************************************************/
		
		
		
		
		
		
		/************************************************************************/
		/*                        激光测距 START                               */
		/************************************************************************/
		
		//连续测32次取平均值
		for(dist_cnt = 0; dist_cnt < 5; dist_cnt++)
		{
			//激光测距执行一次测量
		  Status = vl53l0x_start_single_test(&vl53l0x_dev,&vl53l0x_data);
			dist += Distance_data;
		}
		//printf("Distance: %d\n", Distance_data);
		
		Distance_data = dist/5;
		
		
		if(Status==VL53L0X_ERROR_NONE)
		{
			//没关着，没人
			if(room_status == ROOM_STATUS[0])
			{
				BEEP_OFF;
				User_Fine = 0;
				//LED_RED_Signal = 0;
				show_time_1 = 0;
				//TIM_Cmd(BEEP_TIM, DISABLE);
				LED_GREEN;
				
				sprintf(str_distance_1_available, "Room is available");
				LCD_ShowString(0,96, 480, 24, 24, str_distance_1_available);
				sprintf(str_room_1_time, "                    ");
				LCD_ShowString(0, 124, 480, 24, 24, str_room_1_time);
				
				TIM_Cmd(BASIC_TIM_1_time, DISABLE);
				TIM_Cmd(BASIC_TIM_1_LED, DISABLE);
				//LED_RED_Signal = 0;
				
				ESP8266_SendString ( DISABLE, str_distance_1_available, 17, Multiple_ID_0 );
			}
			//门关着，有人
			else if(room_status == ROOM_STATUS[1])
			{
				BEEP_OFF;
				//TIM_Cmd(BEEP_TIM, DISABLE);
				if(show_time_1 < 15)
				{
					LED_BLUE;
				}
				else if(show_time_1 >= 15)
				{
					LED3_OFF;
				}
				
				sprintf(str_distance_1_busy, "Room is busy       ");
				LCD_ShowString(0,96, 480, 24, 24, str_distance_1_busy);
				
				TIM_Cmd(BASIC_TIM_1_time, ENABLE);
				sprintf(str_room_1_time, "%2d min ago", show_time_1);
				LCD_ShowString(0, 124, 480, 24, 24, str_room_1_time);
				
				strcat(str_distance_1_busy, str_room_1_time);
				ESP8266_SendString ( DISABLE, str_distance_1_busy, 28, Multiple_ID_0 );
				
			}
			//门开着，没人
			else if(room_status == ROOM_STATUS[2])
			{
				BEEP_OFF;
				User_Fine = 0;
				//LED_RED_Signal = 0;
				show_time_1 = 0;
				//TIM_Cmd(BEEP_TIM, DISABLE);
				LED_WHITE;
				
				sprintf(str_distance_1_available, "Room is available");
				LCD_ShowString(0,96, 480, 24, 24, str_distance_1_available);
				sprintf(str_room_1_time, "                    ");
				LCD_ShowString(0, 124, 480, 24, 24, str_room_1_time);
				
				TIM_Cmd(BASIC_TIM_1_time, DISABLE);
				TIM_Cmd(BASIC_TIM_1_LED, DISABLE);
				//LED_RED_Signal = 0;
				
				ESP8266_SendString ( DISABLE, str_distance_1_available, 17, Multiple_ID_0 );
				
			}
			//门开着，有人
			else if(room_status == ROOM_STATUS[3])
			{
				LED_RED;
				//TIM_Cmd(BEEP_TIM, ENABLE);
				BEEP_ON;
				
				sprintf(str_distance_1_busy, "Room is busy       ");
				LCD_ShowString(0,96, 480, 24, 24, str_distance_1_busy);
				
				TIM_Cmd(BASIC_TIM_1_time, ENABLE);
				sprintf(str_room_1_time, "%2d min ago", show_time_1);
				LCD_ShowString(0, 124, 480, 24, 24, str_room_1_time);
				
				strcat(str_distance_1_busy, str_room_1_time);
				ESP8266_SendString ( DISABLE, str_distance_1_busy, 28, Multiple_ID_0 );
			}
			
			#if 0
			//有人
			if(Distance_data < 100)
			{
				sprintf(str_distance_1_busy, "Room is busy       ");
				LCD_ShowString(0,96, 480, 24, 24, str_distance_1_busy);
				
				TIM_Cmd(BASIC_TIM_1_time, ENABLE);
				sprintf(str_room_1_time, "%2d min ago", show_time_1);
				LCD_ShowString(0, 124, 480, 24, 24, str_room_1_time);
				
				strcat(str_distance_1_busy, str_room_1_time);
				ESP8266_SendString ( DISABLE, str_distance_1_busy, 28, Multiple_ID_0 );
				//ESP8266_SendString ( DISABLE, str_room_1_time, 10, Multiple_ID_0 );
				
			}
			//没人
			else
			{
				User_Fine = 0;
				LED_RGBOFF;
				//LED_RED_Signal = 0;
				show_time_1 = 0;
				BEEP_OFF;
				
				sprintf(str_distance_1_available, "Room is available");
				LCD_ShowString(0,96, 480, 24, 24, str_distance_1_available);
				sprintf(str_room_1_time, "                    ");
				LCD_ShowString(0, 124, 480, 24, 24, str_room_1_time);
				
				TIM_Cmd(BASIC_TIM_1_time, DISABLE);
				TIM_Cmd(BASIC_TIM_1_LED, DISABLE);
				//LED_RED_Signal = 0;
				
				ESP8266_SendString ( DISABLE, str_distance_1_available, 17, Multiple_ID_0 );
			}
			#endif
			
			
			
			
			
		}
		else
		{
			LCD_ShowString(0, 96, 480, 24, 24, "Measurement Error!");
		}
		dist = 0;
		
		/************************************************************************/
		/*                     激光测距 END                                   */
		/************************************************************************/
		
		
		
		/************************************************************************/
		/*                     超时警告 START                                   */
		/************************************************************************/
		
//		if(LED_RED_Signal == 1)
//		{
//			LED_RED;
//		}
//		else if(LED_RED_Signal == 0)
//		{
//			LED_RGBOFF;
//		}
		
		/************************************************************************/
		/*                     超时警告 END                                   */
		/************************************************************************/
		
		
		
		
		/************************************************************************/
	  /*                        ESP8266 START                               */
		/************************************************************************/
		
		//ESP8266_SendString ( ENABLE, cStr, 0, Single_ID_0 );
		
		
		
		/************************************************************************/
		/*                        ESP8266 END                               */
		/************************************************************************/
		
	}
	#endif
	
}
/*********************************************END OF FILE**********************/
