#include "hcsr04.h"

extern int echo_time;

static void Hcsr04_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(HCSR04_GPIO_CLK, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = Trig_Pin;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	GPIO_Init(Trig_Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Echo_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(Echo_Port, &GPIO_InitStructure);
	
	
}

static float Hcsr04_Get_ONE_Distance()
{
	float temp_distance = 0;
	
	//printf("High start\n");
	Trig_High;
	delay_us(20);
	Trig_Low;
	//printf("high done\n");
	
	//printf("echo status:%d\n", Echo_Status);
	//̽�⵽�ߵ�ƽ���˳�ѭ��,��ʼ����
	while(Echo_Status == 0);
	TIM_Cmd(Echo_TIM , ENABLE);
	
	//printf("Echo Start\n");
	
	//̽�⵽�͵�ƽ�˳�ѭ����ֹͣ����
	while(Echo_Status == 1);
	TIM_Cmd(Echo_TIM , DISABLE);
	
	//printf("Echo Done\n");
	
	temp_distance = echo_time/58;
	
	echo_time = 0;
	
	return temp_distance;
	
	
}	


float Hcsr04_Get_Distance()
{
	float temp_distance = 0;
	float sum = 0;
	int count = 0;
	for(count = 0; count < 5; count++)
	{
		temp_distance = Hcsr04_Get_ONE_Distance();
		sum += temp_distance;
	}
	
	return sum/5;
}


// �ж����ȼ�����
static void Echo_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // �����ж���Ϊ0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);		
		// �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = Echo_TIM_IRQ ;	
		// ���������ȼ�Ϊ 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	 
	  // ������ռ���ȼ�Ϊ3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static void Echo_TIM_Config(void)
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
		// ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
    Echo_TIM_APBxClock_FUN(Echo_TIM_CLK, ENABLE);
	
		// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
		// ��ʱ��ʱ�� = (psr+1)/72M * ��arr+1��
		// ��ʱʱ��Ϊ ����һ��ʱ��*arr(s), ������1ms
    TIM_TimeBaseStructure.TIM_Period = Echo_TIM_Period;	

	  // ʱ��Ԥ��Ƶ��Ϊ
		// ����һ�ε�ʱ��Ϊ (psr+1)/72M(s), ������1*10^(-6)
    TIM_TimeBaseStructure.TIM_Prescaler = Echo_TIM_Prescaler;
	
		// ʱ�ӷ�Ƶ���� ��������ʱ��û�У����ù�
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
		
		// ����������ģʽ��������ʱ��ֻ�����ϼ�����û�м���ģʽ������
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
		
		// �ظ���������ֵ��������ʱ��û�У����ù�
		TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	
	  // ��ʼ����ʱ��
    TIM_TimeBaseInit(Echo_TIM, &TIM_TimeBaseStructure);
		
		// ����������жϱ�־λ
    TIM_ClearFlag(Echo_TIM, TIM_FLAG_Update);
	  
		// �����������ж�
    TIM_ITConfig(Echo_TIM,TIM_IT_Update,ENABLE);
		
		// ʹ�ܼ�����
    TIM_Cmd(Echo_TIM, ENABLE);
}

static void Echo_TIM_Init(void)
{
	Echo_TIM_NVIC_Config();
	Echo_TIM_Config();
}

void HCSR04_Init(void)
{
	Hcsr04_GPIO_Config();
	Echo_TIM_Init();


}
