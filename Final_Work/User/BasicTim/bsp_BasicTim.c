#include "bsp_BasicTim.h"

// �ж����ȼ�����
static void BASIC_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // �����ж���Ϊ0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);		
		// �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = BASIC_TIM_IRQ_1_time ;	
		// ���������ȼ�Ϊ 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	 
	  // ������ռ���ȼ�Ϊ3
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
		
		// ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
   BASIC_TIM_APBxClock_FUN_1_time(BASIC_TIM_CLK_1_time, ENABLE);
	
		// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
		// ��ʱ��ʱ�� = (psr+1)/72M * ��arr+1��
		// ��ʱʱ��Ϊ ����һ��ʱ��*arr(s), ������1ms
    TIM_TimeBaseStructure.TIM_Period = BASIC_TIM_Period_1_time;	

	  // ʱ��Ԥ��Ƶ��Ϊ
		// ����һ�ε�ʱ��Ϊ (psr+1)/72M(s), ������1*10^(-6)
    TIM_TimeBaseStructure.TIM_Prescaler= BASIC_TIM_Prescaler_1_time;
	
		// ʱ�ӷ�Ƶ���� ��������ʱ��û�У����ù�
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
		
		// ����������ģʽ��������ʱ��ֻ�����ϼ�����û�м���ģʽ������
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
		
		// �ظ���������ֵ��������ʱ��û�У����ù�
		TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	
	  // ��ʼ����ʱ��
    TIM_TimeBaseInit(BASIC_TIM_1_time, &TIM_TimeBaseStructure);
		
		// ����������жϱ�־λ
    TIM_ClearFlag(BASIC_TIM_1_time, TIM_FLAG_Update);
	  
		// �����������ж�
    TIM_ITConfig(BASIC_TIM_1_time,TIM_IT_Update,ENABLE);
		
		// ʹ�ܼ�����
    TIM_Cmd(BASIC_TIM_1_time, ENABLE);
}

static void BASIC_TIM_1_LED_Config(void)
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
    BASIC_TIM_APBxClock_FUN_1_time(BASIC_TIM_CLK_1_LED, ENABLE);
	
    TIM_TimeBaseStructure.TIM_Period = BASIC_TIM_Period_1_LED;	
    TIM_TimeBaseStructure.TIM_Prescaler= BASIC_TIM_Prescaler_1_LED;
	   
	  //һ������������ʱ�������ù�
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
	   
	  //һ������������ʱ�������ù�
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
	   
	  //һ������������ʱ�������ù�
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



