/*!
  * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  * @licence     The MIT License (MIT)
  * @author      PengKaixing(kaixing.peng@dfrobot.com)
  * @version     V0.1
  * @date        2021-05-17
  * @get         from https://www.dfrobot.com
  * @url         https://github.com/dfrobot/DFRobot_Trafficlight
  */
#include "DFRobot_Trafficlight.h"

void DFRobot_TRAFFICLIGHT::getNowTime()
{
  uint8_t buf[10] = {0};
  uint8_t rbuf[12] = {0};
  buf[0] = LIGHT_GET_NOW_TIME;
  sProtocol_t _protocol = pack(buf, sizeof(buf));
  writeReg(0x00, (uint8_t *)&_protocol, sizeof(_protocol));
  readReg(0x00, rbuf, sizeof(rbuf));
  _NowTime.hour = rbuf[2];
  _NowTime.minute = rbuf[3];
  _NowTime.second = rbuf[4];
}

sProtocol_t DFRobot_TRAFFICLIGHT::pack(uint8_t *pBuf, uint8_t len)
{
  sProtocol_t _protocol;
  _protocol.head = 0xff;
  _protocol.tail = 0xee;
  memcpy(_protocol.data, pBuf, len);
  return _protocol;
}

void DFRobot_TRAFFICLIGHT::updateModuleTime(uint8_t hour, uint8_t minute, uint8_t second)
{
  uint8_t buf[10]={0};
  buf[0] = LIGHT_UPDATE_MODULE_TIME;
  buf[1] = hour;
  buf[2] = minute;
  buf[3] = second;
  sProtocol_t _protocol = pack(buf,sizeof(buf));
  writeReg(0x00, (uint8_t* )&_protocol, sizeof(_protocol));
  uint8_t rbuf[12];
  readReg(0x00, rbuf, sizeof(rbuf));
  delay(500);
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
  uint8_t buf[10] = {0};
  buf[0] = LIGHT_SEND_MESSAGE_TO_MCU;
  buf[1] = this->_begin_hour;
  buf[2] = this->_begin_minute;
  buf[3] = this->_begin_second;
  buf[7] = this->_R_time;
  buf[8] = this->_Y_time;
  buf[9] = this->_G_time;
  sProtocol_t _protocol = pack(buf, sizeof(buf));
  writeReg(0x00, (uint8_t *)&_protocol, sizeof(_protocol));
  uint8_t rbuf[12];
  readReg(0x00, rbuf, sizeof(rbuf));
  delay(500);
}

void DFRobot_TRAFFICLIGHT::changeDefaultRYGTime(uint8_t R_time, uint8_t Y_time, uint8_t G_time)
{
  uint8_t buf[10] = {0};
  buf[0] = LIGHT_CHANGE_DEFAULT_LIGHT;
  buf[1] = R_time;
  buf[2] = Y_time;
  buf[3] = G_time;
  sProtocol_t _protocol = pack(buf, sizeof(buf));
  writeReg(0x00, (uint8_t *)&_protocol, sizeof(_protocol));
  uint8_t rbuf[12];
  readReg(0x00, rbuf, sizeof(rbuf));
  // for(int i =0;i<12;i++)
  //   DBG(rbuf[i]);
  delay(500);
}

void DFRobot_TRAFFICLIGHT::clearSchedule()
{
  uint8_t buf[10] = {0};
  buf[0] = LIGHT_CLEAR_SCHEDULE;
  sProtocol_t _protocol = pack(buf, sizeof(buf));
  writeReg(0x00, (uint8_t *)&_protocol, sizeof(_protocol));
  uint8_t rbuf[12];
  readReg(0x00, rbuf, sizeof(rbuf));
  delay(500);
}

bool DFRobot_TRAFFICLIGHT::IfLightIsOn(uint8_t light)
{
  String activelight;
  uint8_t buf[10] = {0};
  buf[0] = LIGHT_GET_WHITCH_LIGHT_IS_ON;
  sProtocol_t _protocol = pack(buf, sizeof(buf));
  writeReg(0x00, (uint8_t *)&_protocol, sizeof(_protocol));
  uint8_t rbuf[12];
  readReg(0x00, rbuf, sizeof(rbuf));
  if (light == rbuf[2])
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