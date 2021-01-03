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
	//探测到高电平才退出循环,开始计数
	while(Echo_Status == 0);
	TIM_Cmd(Echo_TIM , ENABLE);
	
	//printf("Echo Start\n");
	
	//探测到低电平退出循环，停止计数
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


// 中断优先级配置
static void Echo_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);		
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = Echo_TIM_IRQ ;	
		// 设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	 
	  // 设置抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static void Echo_TIM_Config(void)
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
		// 开启定时器时钟,即内部时钟CK_INT=72M
    Echo_TIM_APBxClock_FUN(Echo_TIM_CLK, ENABLE);
	
		// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
		// 定时器时间 = (psr+1)/72M * （arr+1）
		// 计时时间为 计数一次时间*arr(s), 这里是1ms
    TIM_TimeBaseStructure.TIM_Period = Echo_TIM_Period;	

	  // 时钟预分频数为
		// 计数一次的时间为 (psr+1)/72M(s), 这里是1*10^(-6)
    TIM_TimeBaseStructure.TIM_Prescaler = Echo_TIM_Prescaler;
	
		// 时钟分频因子 ，基本定时器没有，不用管
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
		
		// 计数器计数模式，基本定时器只能向上计数，没有计数模式的设置
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
		
		// 重复计数器的值，基本定时器没有，不用管
		TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	
	  // 初始化定时器
    TIM_TimeBaseInit(Echo_TIM, &TIM_TimeBaseStructure);
		
		// 清除计数器中断标志位
    TIM_ClearFlag(Echo_TIM, TIM_FLAG_Update);
	  
		// 开启计数器中断
    TIM_ITConfig(Echo_TIM,TIM_IT_Update,ENABLE);
		
		// 使能计数器
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
