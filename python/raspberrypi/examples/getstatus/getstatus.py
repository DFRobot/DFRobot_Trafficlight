# -*- coding: utf-8 -*
'''
  * @file  getstatus.py
    @n i2c 地址选择，默认i2c地址为0x57，A1、A0组合成4种IIC地址
                | A1 | A0 |
                | 0  | 0  |    0x54
                | 0  | 1  |    0x55
                | 1  | 0  |    0x56
                | 1  | 1  |    0x57   default i2c address  
  * @n 实验现象：  获取此时灯的状态
  * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  * @licence     The MIT License (MIT)
  * @author      PengKaixing(kaixing.peng@dfrobot.com)
  * @version     V1.0
  * @date        2021-05-17
  * @get         from https://www.dfrobot.com
  * @url         https://github.com/dfrobot/DFRobot_Trafficlight
'''
import sys
import os
import time

sys.path.append(os.path.dirname(os.path.dirname(os.path.dirname(os.path.realpath(__file__)))))
from dfrobot_trafficlight import *

I2C_1       = 0x01               # I2C_1 使用i2c1接口驱动传感器， 可以调整为i2c0但是需要配置树莓派的文件
I2C_ADDRESS = 0x55               # I2C 设备的地址，可以更改A1、A0来更换地址，默认地址为0x54
trafficlight = dfrobot_trafficlight_I2C(I2C_1 ,I2C_ADDRESS)


def setup():
  '''
   *  @brief 修改交通灯默认持续值
   *  @param 参数1：红灯持续时间 参数2：黄灯持续时间  参数3：绿灯持续时间
             范围：1~99
   *         单位：秒
   '''
  trafficlight.change_default_RYG_time(11,3,11)
  
  '''
   *  @brief 设置从0时1分0开始，持续亮红灯
   *  @param 范围：1~99
   *         单位：秒
   '''
  trafficlight.clear_schedule()
  trafficlight.set_begin_time(0,0,20)
  trafficlight.set_RYG_light_time(1,0,0)
  trafficlight.send_message_to_MCU()
  trafficlight.update_module_time(0,0,0)
def loop():
  '''
   *  @param RED_LIGHT    
   *         YELLOW_LIGHT 
   *         GREEN_LIGHT 
   '''
  if (1 == trafficlight.if_light_is_on(trafficlight.RED_LIGHT)):
    print("light is on!")
  else:
    print("light is off!")
  time.sleep(1) 

if __name__ == "__main__":
  setup()
  while True:
    loop()