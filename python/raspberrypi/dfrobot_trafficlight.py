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
  #用作设置交通灯在某个时间段开始按照想要的亮灯方式工作
  _begin_hour = 0
  _begin_minute = 0
  _begin_second = 0

  _R_time = 0
  _Y_time = 0
  _G_time = 0
  
  #用作存储交通灯此时的时间
  hour = 0
  minute = 0
  second = 0
  
  RED_LIGHT = 0X00
  YELLOW_LIGHT = 0X01
  GREEN_LIGHT = 0X02
  #设置交通灯某一个灯常亮
  LIGHT_INFINITY_TIME = 0xff
  
  def __init__(self ,bus ,Baud):
    if bus != 0:
      self.i2cbus = smbus.SMBus(bus)
      self.__uart_i2c = I2C_MODE 
  '''
   *  @brief 清空MCU时刻表，时刻表是用来记录红绿灯在不同时间段红绿黄三个灯持续亮的时间
   *  @param NULL
   *  @return 没有返回值
  '''
  def clear_schedule(self):
    clear_mark = [0]*1
    clear_mark[0]=1
    self.write_reg(0x11, clear_mark)
    time.sleep(0.5)

  '''
   *  @brief 设置交通灯状态改变的起始时间
   *  @param hour,minute,second
   *  @return 没有返回值
  '''
  def set_begin_time(self,begin_hour,begin_minute,begin_second):
    self._begin_hour = begin_hour
    self._begin_minute = begin_minute
    self._begin_second = begin_second

  '''
   *  @brief 设置三种灯持续亮的时间
   *  @param R_time:红灯持续时间
   *         Y_time:黄灯持续时间
   *         G_time:绿灯持续时间
   *  @return 没有返回值
  '''
  def set_RYG_light_time(self, R_time, Y_time, G_time):
    if(R_time == self.LIGHT_INFINITY_TIME):
      self._R_time = 1
      self._Y_time = 0
      self._G_time = 0
      return
    elif (Y_time == self.LIGHT_INFINITY_TIME):
      self._R_time = 0
      self._Y_time = 1
      self._G_time = 0
      return
    elif (G_time == self.LIGHT_INFINITY_TIME):
      self._R_time = 0
      self._Y_time = 0
      self._G_time = 1
      return
    else:
      self._R_time = R_time
      self._Y_time = Y_time
      self._G_time = G_time

  '''
   *  @brief 发送设置到MCU
   *  @param NULL
   *  @return 没有返回值
  '''
  def send_message_to_MCU(self):
    mark =[0]*1
    mark[0]=1
    self.write_reg(0x0A, mark)
    time.sleep(0.5)
    time_buf = [0]*3
    ryg_buf = [0]*3
    time_buf[0] = self._begin_hour
    time_buf[1] = self._begin_minute
    time_buf[2] = self._begin_second
    self.write_reg(0x17, time_buf)
    time.sleep(0.5)
    ryg_buf[0] = self._R_time;
    ryg_buf[1] = self._Y_time;
    ryg_buf[2] = self._G_time;
    self.write_reg(0x0B, ryg_buf)
    time.sleep(0.5)

  '''
   *  @brief 更新MCU的时间
   *  @param hour,minute,second
   *  @return 没有返回值
  ''' 
  def update_module_time(self,hour,minute,second):
    mark = [0]*1
    buf = [0]*3
    mark[0]=1
    self.write_reg(0x09, mark)
    time.sleep(0.5)
    buf[0] = hour
    buf[1] = minute
    buf[2] = second
    self.write_reg(0x06, buf)
    time.sleep(0.5)

  '''
   *  @brief 获取交通灯此时的状态
   *  @param light:设置想要检测的灯
   *  @return true:  亮
   *          false：灭
  '''
  def if_light_is_on(self,light):
    now_light = self.read_reg(0x12, 1)
    if (light == now_light[0]):
      return 1
    else: 
      return 0
  '''
   *  @brief 获取此时MCU的时间
   *  @param NULL
   *  @return 没有返回值
  '''
  def get_now_time(self):
    rbuf = self.read_reg(0x0E,3)
    self.hour = rbuf[0]
    self.minute = rbuf[1]
    self.second = rbuf[2]

  '''
   *  @brief 修改8S默认的红黄绿三色灯的持续时间
   *  @param  R_time:红灯持续时间
   *         Y_time:黄灯持续时间
   *         G_time:绿灯持续时间
   *  @return NULL
  '''
  def change_default_RYG_time(self,R_time, Y_time, G_time):
    default_mark = [0]*1
    default_mark[0]=1
    self.write_reg(0x13, default_mark)
    time.sleep(0.5)
    buf = [0]*3 
    buf[0] = R_time
    buf[1] = Y_time
    buf[2] = G_time
    self.write_reg(0x14, buf)
      
#brief An example of an i2c interface module
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
    
