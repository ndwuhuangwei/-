/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTI
  
  AL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "bsp_BasicTim.h"
#include "bsp_led.h"
#include "sys.h"
#include "delay.h"
#include "bsp_key.h"
#include "bsp_usart.h"
#include "beep.h"
#include "bsp_esp8266.h"
#include "vl53l0x.h"
#include "hcsr04.h"

extern int show_time_1;
extern int show_time_2;
extern int time_1_counter;
extern int time_2_counter;
extern int LED_1_counter;
extern int LED_2_counter;
extern int LED_RED_Signal;
extern int unit_time;
extern int unit_LED;
extern int User_Fine;

extern VL53L0X_Error Status;
extern vu16 Distance_data;
extern int ROOM_STATUS[4];
extern int room_status;
extern uint16_t UltraWave_Distance;

extern int laser_safe_distance;
extern int UltraWave_safe_distance;

extern uint32_t BEEP_Counter;
extern int BEEP_Interval;
extern int echo_time;

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}





/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/


void BASIC_TIM_IRQHandler_1_time(void)
{
	if ( TIM_GetITStatus( BASIC_TIM_1_time, TIM_IT_Update) != RESET ) 
	{	
		//LED_PURPLE;
		time_1_counter ++;
		if(time_1_counter > unit_time)
		{
			time_1_counter = 0;
		}
		
		if(time_1_counter == unit_time)
		{
			show_time_1 ++;
			time_1_counter = 0;
		}
		
		
//		if(LED_RED_Signal == 1)
//		{
//			LED_RED;
//		}
//		else if(LED_RED_Signal == 0)
//		{
//			LED_RGBOFF;
//		}
		
		
		if((show_time_1 > 15 || show_time_1 == 15) && show_time_1 < 30 && room_status != ROOM_STATUS[3])
		{
			TIM_Cmd(BASIC_TIM_1_LED, ENABLE);
			if(show_time_1 == 29)
			{
				User_Fine = 0;
			}
			
		}
		
		if(show_time_1 >= 30 && User_Fine == 0 )
		{
			TIM_Cmd(BASIC_TIM_1_LED, DISABLE);
			//LED_RED_Signal = 1;
			LED_RED;
			BEEP_ON;
		}
		else if(User_Fine == 1)
		{
			BEEP_OFF;
		}
		
		
		
		TIM_ClearITPendingBit(BASIC_TIM_1_time , TIM_FLAG_Update);  		 
	}		 	
}


void BASIC_TIM_IRQHandler_1_LED(void)
{
	if ( TIM_GetITStatus( BASIC_TIM_1_LED, TIM_IT_Update) != RESET ) 
	{	
		LED_1_counter ++;
		if(LED_1_counter > unit_LED)
		{
			LED_1_counter = 0;
		}
		
		if (LED_1_counter == unit_LED)
		{
			LED_1_counter = 0;
			
			//在1和0之间切换
//			LED_RED_Signal = LED_RED_Signal ^ 1;
			
			LED1_TOGGLE;
		}
		
		TIM_ClearITPendingBit(BASIC_TIM_1_LED, TIM_FLAG_Update);  		 
	}		 	
}

void macESP8266_USART_INT_FUN ( void )
{	
	uint8_t ucCh;
	
	
	if ( USART_GetITStatus ( macESP8266_USARTx, USART_IT_RXNE ) != RESET )
	{
		ucCh  = USART_ReceiveData( macESP8266_USARTx );
		
		if ( strEsp8266_Fram_Record .InfBit .FramLength < ( RX_BUF_MAX_LEN - 1 ) )                       //预留1个字节写结束符
			strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ++ ]  = ucCh;

	}
	 	 
	if ( USART_GetITStatus( macESP8266_USARTx, USART_IT_IDLE ) != RESET )                                         //数据帧接收完毕
	{
    strEsp8266_Fram_Record .InfBit .FramFinishFlag = 1;
		
		ucCh = USART_ReceiveData( macESP8266_USARTx );                                                              //由软件序列清除中断标志位(先读USART_SR，然后读USART_DR)	
		
  }	

}

void KEY1_IRQHandler(void)
{
  //确保是否产生了EXTI Line中断
	if(EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET) 
	{
//		ENUM_ID_NO_TypeDef ID;
//		for(ID = Multiple_ID_0; ID <= Multiple_ID_4; ID++)
//		{
		ESP8266_SendString ( DISABLE, "HELP! Toilet papaer please!", 26, Multiple_ID_0 );
//		}
		
		
		
    //清除中断标志位
		EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);     
	}  
}

void KEY2_IRQHandler(void)
{
  //确保是否产生了EXTI Line中断
	if(EXTI_GetITStatus(KEY2_INT_EXTI_LINE) != RESET) 
	{
		User_Fine = 1;
		BEEP_OFF;
		
    //清除中断标志位
		EXTI_ClearITPendingBit(KEY2_INT_EXTI_LINE);     
	}  
}



void ROOM_STATUS_TIM_IRQHandler(void)
{
	if ( TIM_GetITStatus( ROOM_STATUS_TIM, TIM_IT_Update) != RESET ) 
	{	
		
		//门关着
		if(UltraWave_Distance < UltraWave_safe_distance)
		{
			//没人
			if(Distance_data > laser_safe_distance)
			{
				//绿灯亮
				room_status = ROOM_STATUS[0];
			}
			//有人
			else
			{
				//蓝灯亮
				room_status = ROOM_STATUS[1];
			}
		}
		//门开着
		else
		{
			//没人
			if(Distance_data > laser_safe_distance)
			{
				//白灯亮
				room_status = ROOM_STATUS[2];
			}
			else
			{
				//蜂鸣器间隔响
				room_status = ROOM_STATUS[3];
			}
		}
		
		TIM_ClearITPendingBit(ROOM_STATUS_TIM, TIM_FLAG_Update);  		 
	}	
}

void BEEP_TIM_IRQHandler(void)
{
	if ( TIM_GetITStatus( BEEP_TIM, TIM_IT_Update) != RESET ) 
	{	
		BEEP_Counter ++;
		if(BEEP_Counter > BEEP_Interval)
		{
			BEEP_Counter = 0;
		}
		
		if (BEEP_Counter == BEEP_Interval)
		{
			BEEP_Counter = 0;
			//printf("\n aaaa \n");
			BEEP_TOGGLE;
		}
		
		TIM_ClearITPendingBit(BEEP_TIM, TIM_FLAG_Update);  		 
	}	
}

void Echo_TIM_IRQHandler(void)
{
	if ( TIM_GetITStatus( Echo_TIM, TIM_IT_Update) != RESET ) 
	{	
		echo_time++;
		//printf("echo_time ++\n");
		//delay_ms(100);
		
		TIM_ClearITPendingBit( Echo_TIM , TIM_FLAG_Update);  		 
	}
}

