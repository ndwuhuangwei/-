#include "bsp_key.h"

static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* ����NVICΪ���ȼ���1 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  
  /* �����ж�Դ������1 */
  NVIC_InitStructure.NVIC_IRQChannel = KEY1_INT_EXTI_IRQ;
  /* ������ռ���ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  /* ���������ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* ʹ���ж�ͨ�� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* �����ж�Դ������2������ʹ������������� */  
  NVIC_InitStructure.NVIC_IRQChannel = KEY2_INT_EXTI_IRQ;
  NVIC_Init(&NVIC_InitStructure);
}


void EXTI_Key_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(KEY2_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);	
	
	NVIC_Configuration();
	
	
	// KEY1����
	GPIO_InitStruct.GPIO_Pin = KEY1_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;	
	GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStruct);
	
	GPIO_EXTILineConfig(KEY1_INT_EXTI_PORTSOURCE, KEY1_INT_EXTI_PINSOURCE);
	
	EXTI_InitStructure.EXTI_Line = KEY1_INT_EXTI_LINE;
	/* EXTIΪ�ж�ģʽ */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* �������ж� */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  /* ʹ���ж� */	
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

	// KEY2����
	GPIO_InitStruct.GPIO_Pin = KEY2_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;	
	GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStruct);	

	/* ѡ��EXTI���ź�Դ */
  GPIO_EXTILineConfig(KEY2_INT_EXTI_PORTSOURCE, KEY2_INT_EXTI_PINSOURCE); 
  
	EXTI_InitStructure.EXTI_Line = KEY2_INT_EXTI_LINE;
	/* EXTIΪ�ж�ģʽ */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* �½����ж� */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  /* ʹ���ж� */	
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
}

uint8_t Key_Scan(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin)
{
	if( GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == KEY_ON )
	{
		// ���ּ��
		while( GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == KEY_ON );
		return KEY_ON;
	}
	else return KEY_OFF;
}

