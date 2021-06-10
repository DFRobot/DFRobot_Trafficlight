#include "DFRobot_Trafficlight.h"

void DFRobot_TRAFFICLIGHT::getNowTime()
{
  uint8_t rbuf[3] = {0};
  readReg(0x0E, rbuf, sizeof(rbuf));
  _NowTime.hour = rbuf[0];
  _NowTime.minute = rbuf[1];
  _NowTime.second = rbuf[2];
}

void DFRobot_TRAFFICLIGHT::updateModuleTime(uint8_t hour, uint8_t minute, uint8_t second)
{
  uint8_t mark = 1;
  uint8_t buf[3]={0};
  writeReg(0x09, &mark, 1);
  delay(100);
  buf[0] = hour;
  buf[1] = minute;
  buf[2] = second;
  writeReg(0x06, buf, sizeof(buf));
}

void DFRobot_TRAFFICLIGHT::setBeginTime(uint8_t begin_hour, uint8_t begin_minute, uint8_t begin_second)
{
  this->_begin_hour = begin_hour;
  this->_begin_minute = begin_minute;
  this->_begin_second = begin_second;
}

void DFRobot_TRAFFICLIGHT::setRYGLightTime(uint8_t R_time, uint8_t Y_time, uint8_t G_time)
{
  if(R_time == LIGHT_INFINITY_TIME)
  {
    this->_R_time = 1;
    this->_Y_time = 0;
    this->_G_time = 0;
    return;
  }else if (Y_time == LIGHT_INFINITY_TIME)
  {
    this->_R_time = 0;
    this->_Y_time = 1;
    this->_G_time = 0;
    return;
  }else if (G_time == LIGHT_INFINITY_TIME)
  {
    this->_R_time = 0;
    this->_Y_time = 0;
    this->_G_time = 1;
    return;
  }else{
    this->_R_time = R_time;
    this->_Y_time = Y_time;
    this->_G_time = G_time;
  }
}

void DFRobot_TRAFFICLIGHT::sendMessageToMCU()
{
  uint8_t mark = 1;
  writeReg(0x0A, &mark, 1);
  delay(100);
  uint8_t time_buf[3] = {0};
  uint8_t ryg_buf[3] = {0};
  time_buf[0] = this->_begin_hour;
  time_buf[1] = this->_begin_minute;
  time_buf[2] = this->_begin_second;
  writeReg(0x17, time_buf, sizeof(time_buf));
  delay(100);
  ryg_buf[0] = this->_R_time;
  ryg_buf[1] = this->_Y_time;
  ryg_buf[2] = this->_G_time;
  writeReg(0x0B, ryg_buf, sizeof(ryg_buf));
}

void DFRobot_TRAFFICLIGHT::changeDefaultRYGTime(uint8_t R_time, uint8_t Y_time, uint8_t G_time)
{
  uint8_t default_mark = 1;
  writeReg(0x13, &default_mark, 1);
  delay(100);
  uint8_t buf[3] = {0};
  buf[0] = R_time;
  buf[1] = Y_time;
  buf[2] = G_time;
  writeReg(0x14, buf, sizeof(buf));
}

void DFRobot_TRAFFICLIGHT::clearSchedule()
{
  uint8_t clear_mark = 1;
  writeReg(0x11, &clear_mark, 1);
}

bool DFRobot_TRAFFICLIGHT::ifLightIsOn(uint8_t light)
{
  uint8_t now_light;
  readReg(0x12, &now_light,1);
  if (light == now_light)
    return 1;
  else 
    return 0;
}

//IIC底层通信
DFRobot_TRAFFICLIGHT_I2C::DFRobot_TRAFFICLIGHT_I2C(TwoWire *pWire, uint8_t addr)
{
  _pWire = pWire;
  this->_I2C_addr = addr;
}

bool DFRobot_TRAFFICLIGHT_I2C::begin()
{
  _pWire->begin();
  _pWire->beginTransmission(_I2C_addr);
  if(_pWire->endTransmission() == 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void DFRobot_TRAFFICLIGHT_I2C::writeReg(uint8_t Reg, void *pData, uint8_t len)
{
  uint8_t* Data = (uint8_t *)pData;
  _pWire->beginTransmission(this->_I2C_addr);
  _pWire->write(Reg);
  for(uint8_t i = 0; i < len; i++)
  {
    _pWire->write(Data[i]);
  }
  _pWire->endTransmission();
}

int16_t DFRobot_TRAFFICLIGHT_I2C::readReg(uint8_t Reg, uint8_t *Data, uint8_t len)
{
  int i=0;
  _pWire->beginTransmission(this->_I2C_addr);
  _pWire->write(Reg);
  if(_pWire->endTransmission() != 0)
  {
    return -1;
  }
  _pWire->requestFrom((uint8_t)this->_I2C_addr,(uint8_t)len);
  while (_pWire->available())
  {
    Data[i++]=_pWire->read();
  }
  return len;
}