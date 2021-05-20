# -*- coding: utf-8 -*
'''
  * @file  getstatus.ino
    @n i2c ��ַѡ��Ĭ��i2c��ַΪ0x57��A1��A0��ϳ�4��IIC��ַ
                | A1 | A0 |
                | 0  | 0  |    0x54
                | 0  | 1  |    0x55
                | 1  | 0  |    0x56
                | 1  | 1  |    0x57   default i2c address  
  * @n ʵ������  ��ȡ��ʱ�Ƶ�״̬
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
from dfrobot_rafficlight import *

I2C_1       = 0x01               # I2C_1 ʹ��i2c1�ӿ������������� ���Ե���Ϊi2c0������Ҫ������ݮ�ɵ��ļ�
I2C_ADDRESS = 0x54               # I2C �豸�ĵ�ַ�����Ը���A1��A0��������ַ��Ĭ�ϵ�ַΪ0x54
trafficlight = dfrobot_trafficlight_I2C(I2C_1 ,I2C_ADDRESS)

active_light = 0

def setup():
 trafficlight.clear_schedule()
 trafficlight.set_begin_time(0,0,20)
 trafficlight.set_RYG_light_time(1,0,0)
 trafficlight.send_message_to_MCU()
 trafficlight.update_module_time(0,0,0)
def loop():
  global active_light
  if(active_light != trafficlight.get_whitch_light_is_on()):
    active_light = trafficlight.get_whitch_light_is_on()
    if active_light == 0:
      print "red_light"
    elif active_light == 1:
      print "yellow_light"
    elif active_light == 2:
      print "green_light"
  time.sleep(1) 

if __name__ == "__main__":
  setup()
  while True:
    loop()