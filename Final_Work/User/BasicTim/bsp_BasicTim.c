#include "bsp_BasicTim.h"

// 中断优先级配置
static void BASIC_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);		
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = BASIC_TIM_IRQ_1_time ;	
		// 设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	 
	  // 设置抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = BASIC_TIM_IRQ_1_LED;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
		
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = ROOM_STATUS_TIM_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

static void BASIC_TIM_1_time_Config(void)
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
		// 开启定时器时钟,即内部时钟CK_INT=72M
   BASIC_TIM_APBxClock_FUN_1_time(BASIC_TIM_CLK_1_time, ENABLE);
	
		// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
		// 定时器时间 = (psr+1)/72M * （arr+1）
		// 计时时间为 计数一次时间*arr(s), 这里是1ms
    TIM_TimeBaseStructure.TIM_Period = BASIC_TIM_Period_1_time;	

	  // 时钟预分频数为
		// 计数一次的时间为 (psr+1)/72M(s), 这里是1*10^(-6)
    TIM_TimeBaseStructure.TIM_Prescaler= BASIC_TIM_Prescaler_1_time;
	
		// 时钟分频因子 ，基本定时器没有，不用管
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
		
		// 计数器计数模式，基本定时器只能向上计数，没有计数模式的设置
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
		
		// 重复计数器的值，基本定时器没有，不用管
		TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	
	  // 初始化定时器
    TIM_TimeBaseInit(BASIC_TIM_1_time, &TIM_TimeBaseStructure);
		
		// 清除计数器中断标志位
    TIM_ClearFlag(BASIC_TIM_1_time, TIM_FLAG_Update);
	  
		// 开启计数器中断
    TIM_ITConfig(BASIC_TIM_1_time,TIM_IT_Update,ENABLE);
		
		// 使能计数器
    TIM_Cmd(BASIC_TIM_1_time, ENABLE);
}

static void BASIC_TIM_1_LED_Config(void)
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
    BASIC_TIM_APBxClock_FUN_1_time(BASIC_TIM_CLK_1_LED, ENABLE);
	
    TIM_TimeBaseStructure.TIM_Period = BASIC_TIM_Period_1_LED;	
    TIM_TimeBaseStructure.TIM_Prescaler= BASIC_TIM_Prescaler_1_LED;
	   
	  //一下三个基本定时器都不用管
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
		TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	
    TIM_TimeBaseInit(BASIC_TIM_1_LED, &TIM_TimeBaseStructure);
		
    TIM_ClearFlag(BASIC_TIM_1_LED, TIM_FLAG_Update);
	  
    TIM_ITConfig(BASIC_TIM_1_LED,TIM_IT_Update,ENABLE);
		
    TIM_Cmd(BASIC_TIM_1_LED, ENABLE);
}


static void ROOM_STATUS_TIM_Config(void)
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
    ROOM_STATUS_TIM_APBxClock_FUN(ROOM_STATUS_TIM_CLK, ENABLE);
	
    TIM_TimeBaseStructure.TIM_Period = ROOM_STATUS_TIM_Period;	
    TIM_TimeBaseStructure.TIM_Prescaler= ROOM_STATUS_TIM_Prescaler;
	   
	  //一下三个基本定时器都不用管
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
		TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	
    TIM_TimeBaseInit(ROOM_STATUS_TIM, &TIM_TimeBaseStructure);
		
    TIM_ClearFlag(ROOM_STATUS_TIM, TIM_FLAG_Update);
	  
    TIM_ITConfig(ROOM_STATUS_TIM,TIM_IT_Update,ENABLE);
		
    TIM_Cmd(ROOM_STATUS_TIM, ENABLE);
}

static void BEEP_TIM_Config(void)
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
    BEEP_TIM_APBxClock_FUN(BEEP_TIM_CLK, ENABLE);
	
    TIM_TimeBaseStructure.TIM_Period = BEEP_TIM_Period;	
    TIM_TimeBaseStructure.TIM_Prescaler= BEEP_TIM_Prescaler;
	   
	  //一下三个基本定时器都不用管
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
		TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	
    TIM_TimeBaseInit(BEEP_TIM, &TIM_TimeBaseStructure);
		
    TIM_ClearFlag(BEEP_TIM, TIM_FLAG_Update);
	  
    TIM_ITConfig(BEEP_TIM,TIM_IT_Update,ENABLE);
		
    TIM_Cmd(BEEP_TIM, ENABLE);
}


void TIM_Init(void)
{
	BASIC_TIM_NVIC_Config();
	BASIC_TIM_1_time_Config();
	BASIC_TIM_1_LED_Config();
	ROOM_STATUS_TIM_Config();
	//BEEP_TIM_Config();
}



