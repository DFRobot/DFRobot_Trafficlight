# -*- coding: utf-8 -*
'''
  * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  * @licence     The MIT License (MIT)
  * @author      PengKaixing(kaixing.peng@dfrobot.com)
  * @version     V0.1
  * @date        2021-05-17
  * @get         from https://www.dfrobot.com
  * @url         https://github.com/dfrobot/DFRobot_Trafficlight
'''

import time
import smbus
import spidev
import os
import math
import RPi.GPIO as GPIO

I2C_MODE                  = 0x01

class dfrobot_trafficlight(object):

  LIGHT_UPDATE_MODULE_TIME      = 0x01
  LIGHT_SEND_MESSAGE_TO_MCU     = 0x02
  LIGHT_GET_NOW_TIME            = 0x03
  LIGHT_CLEAR_SCHEDULE          = 0x04
  LIGHT_GET_WHITCH_LIGHT_IS_ON  = 0x05

  _begin_hour = 0
  _begin_minute = 0
  _begin_second = 0

  _R_time = 0
  _Y_time = 0
  _G_time = 0
  
  hour = 0
  minute = 0
  second = 0
  
  def __init__(self ,bus ,Baud):
    if bus != 0:
      self.i2cbus = smbus.SMBus(bus)
      self.__uart_i2c = I2C_MODE 

  def clear_schedule(self):
    buf=[0]*12
    buf[0] = 0xff
    buf[1] = self.LIGHT_CLEAR_SCHEDULE
    buf[11] = 0xee
    self.write_reg(0x00,buf)
    rbuf = self.read_reg(0x00,12)
    time.sleep(0.5)

  def set_begin_time(self,begin_hour,begin_minute,begin_second):
    self._begin_hour = begin_hour
    self._begin_minute = begin_minute
    self._begin_second = begin_second

  def set_RYG_light_time(self, R_time, Y_time, G_time):
    self._R_time = R_time
    self._Y_time = Y_time
    self._G_time = G_time

  def send_message_to_MCU(self):
    buf = [0]*12
    buf[0] = 0xff
    buf[1] = self.LIGHT_SEND_MESSAGE_TO_MCU
    buf[2] = self._begin_hour
    buf[3] = self._begin_minute
    buf[4] = self._begin_second
    buf[8] = self._R_time
    buf[9] = self._Y_time
    buf[10] = self._G_time
    buf[11] = 0xee
    self.write_reg(0x00, buf)
    rbuf = self.read_reg(0x00,12);
    time.sleep(0.5)
   
  def update_module_time(self,hour,minute,second):
    buf = [0]*12
    buf[0] = 0xff
    buf[1] = self.LIGHT_UPDATE_MODULE_TIME
    buf[2] = hour;
    buf[3] = minute;
    buf[4] = second;
    buf[11] = 0xee
    self.write_reg(0x00,buf)
    rbuf = self.read_reg(0x00,12)
    time.sleep(0.5)  

  def get_whitch_light_is_on(self):
    buf = [0]*12
    buf[0] = 0xff
    buf[1] = self.LIGHT_GET_WHITCH_LIGHT_IS_ON
    buf[11] = 0xee
    self.write_reg(0x00,buf)
    rbuf = self.read_reg(0x00,12)
    time.sleep(0.5)
    return rbuf[2]

  def get_now_time(self):
    buf = [0]*12
    buf[0] = 0xff
    buf[1] = self.LIGHT_GET_NOW_TIME
    buf[11] = 0xee
    self.write_reg(0x00,buf)
    rbuf = self.read_reg(0x00,12)
    self.hour = rbuf[2]
    self.minute = rbuf[3]
    self.second = rbuf[4]
      
'''
  @brief An example of an i2c interface module
'''
class dfrobot_trafficlight_I2C(dfrobot_trafficlight):
  def __init__(self ,bus ,addr):
    self.__addr = addr
    super(dfrobot_trafficlight_I2C, self).__init__(bus,0)

  '''
    @brief writes data to a register
    @param reg register address
    @param value written data
  '''
  def write_reg(self, reg, data):
    while 1:
      try:
        self.i2cbus.write_i2c_block_data(self.__addr ,reg ,data)
        return
      except:
        print("please check connect!")
        #os.system('i2cdetect -y 1')
        time.sleep(1)
        return
  '''
    @brief read the data from the register
    @param reg register address
    @param value read data
  '''
  def read_reg(self, reg ,len):
    try:
      rslt = self.i2cbus.read_i2c_block_data(self.__addr ,reg ,len)
      #print rslt
    except:
      rslt = -1
    return rslt
    
