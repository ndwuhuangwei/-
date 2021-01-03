tftlcd屏接线
################################################
LCD_PWR:	PG8
LCD_RST:	PG6
LCD_WR:		PB14
LCD_CS:		PG7

//以下两个对应着SPI2的接口，不可随意更改
SCK:      PB13
SDA:      PB15
################################################



激光测距
/******************************************************************************
//1、VL53L0X\platform\inc\vl53l0x_i2c.h  定义SDA和SCL
//2、VL53L0X\platform\src\vl53l0x_i2c.c  初始化SDA和SCL   void VL53L0X_i2c_init(void)
//3、VL53L0X\demo\vl53l0x_it.c           定义上下限距离和INT引脚初始化及中断服务函数


SDA：PA6

SCL：PA7

GPIO：PA5，GPIO是中断输入引脚，查询方式可以不用。

Xshut：不用

程序修改说明
一、修改IIC接口引脚
    1、vl53l0x_i2c.c文件中的 void VL53L0X_i2c_init(void)，配置SCL和SDA引脚
    2、vl53l0x_i2c.h文件中的“IO方向设置”
二、修改中断引脚（查询方式不用修改）
    1、修改vl53l0x_it.c文件中的 static void exti_init(void)，配置GPIO引脚和中断
    2、修改中断入口函数，保证与上面配置一致
三、修改LED指示灯引脚
    1、在main.c文件中修改LED引脚配置和引脚反转定义
四、选择对应的单片机型号并修改启动文件，以及下载程序配置等等

******************************************************************************/


/*LCD屏坐标*/
横边X轴
竖边Y轴
