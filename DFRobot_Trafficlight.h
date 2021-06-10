
/*!
  * @file DFRobot_Trafficlight.h
  * @brief 该库可以设置设备成为现实中的红绿灯的工作方式进行工作
  * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  * @licence     The MIT License (MIT)
  * @author      PengKaixing(kaixing.peng@dfrobot.com)
  * @version     V0.1
  * @date        2021-05-17
  * @get         from https://www.dfrobot.com
  * @url         https://github.com/dfrobot/DFRobot_Trafficlight
  */
#ifndef __DFRobot_TRAFFICLIGHT_H__
#define __DFRobot_TRAFFICLIGHT_H__
#include "Arduino.h"
#include <Wire.h>

// Open this macro to see the program running in detail
#define ENABLE_DBG

#ifdef ENABLE_DBG
#define DBG(...)                     \
    {                                \
        Serial.print("[");           \
        Serial.print(__FUNCTION__);  \
        Serial.print("(): ");        \
        Serial.print(__LINE__);      \
        Serial.print(" ] 0x");         \
        Serial.println(__VA_ARGS__,HEX); \
    }
#else
#define DBG(...)
#endif

/*
  寄存器定义
  * ---------------------------------------------------------------------------------
  * | REG0(R) | REG1(R) | REG2(R) | REG3(R) | REG4(R) | REG5(R) | REG6(R\W)  | REG7(R\W) 
  * ---------------------------------------------------------------------------------
  * | reserve | reserve | reserve | reserve | reserve | reserve |  更新当前时| 更新当前分
  * ---------------------------------------------------------------------------------
  * 
  * ---------------------------------------------------------------------------------
  * | REG8(R\W)|          REG9(R\W)             |           REG10(R\W)
  * ---------------------------------------------------------------------------------
  * |更新当前秒| 更新当前时间（1：更新0：不更新）| 新增时刻表信息标志位（1：有新增0：无新增）
  * ---------------------------------------------------------------------------------
  * 
  * ---------------------------------------------------------------------------------
  * |   REG11(R\W) |   REG12(R\W) |  REG13(R\W) | REG14(R) | REG15(R) | REG16(R) 
  * ---------------------------------------------------------------------------------
  * | 红灯持续时间 | 黄灯持续时间 | 绿灯持续时间 |  当前时  |  当前分  |  当前秒
  * ---------------------------------------------------------------------------------
  * 
  * ---------------------------------------------------------------------------------
  * |  REG17(R)  |     REG18(R)    |            REG19(R) 
  * ---------------------------------------------------------------------------------
  * | 清空时刻表 | 当前正在运行的灯 | 更新交通灯默认运行时间标志位（1：更新0：不更新）
  * ---------------------------------------------------------------------------------
  * 
  * ---------------------------------------------------------------------------------
  * |     REG20(R\W)   |    REG21(R\W)   |    REG22(R\W)    | REG23(R\W) | REG24(R\W)
  * ---------------------------------------------------------------------------------
  * | 默认红灯持续时间 | 默认黄灯持续时间 | 默认绿灯持续时间 |   起始时   |  起始分  
  * ---------------------------------------------------------------------------------
  * 
  * ---------------------------------------------------------------------------------
  * | REG25(R\W) 
  * ---------------------------------------------------------------------------------
  * |  起始秒  
  * ---------------------------------------------------------------------------------
*/
#define RED_LIGHT 0X00
#define YELLOW_LIGHT 0X01
#define GREEN_LIGHT 0X02
#define LIGHT_INFINITY_TIME 0xff

typedef struct
{
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
} sNowTime_t;

class DFRobot_TRAFFICLIGHT
{
  public:
    sNowTime_t _NowTime;
    DFRobot_TRAFFICLIGHT(){};
    ~DFRobot_TRAFFICLIGHT(){};
/*!
 *  @brief 获取此时MCU的时间
 *  @param NULL
 *  @return 没有返回值
 */
    void getNowTime();

/*!
 *  @brief 更新MCU的时间
 *  @param hour,minute,second
 *  @return 没有返回值
 */
    void updateModuleTime(uint8_t hour,uint8_t minute,uint8_t second);

/*!
 *  @brief 设置交通灯状态改变的起始时间
 *  @param hour,minute,second
 *  @return 没有返回值
 */
    void setBeginTime(uint8_t begin_hour, uint8_t begin_minute, uint8_t begin_second);
/*!
 *  @brief 清空MCU时刻表，时刻表是用来记录红绿灯在不同时间段红绿黄三个灯持续亮的时间
 *  @param NULL
 *  @return 没有返回值
 */
    void clearSchedule();

/*!
 *  @brief 设置三种灯持续亮的时间
 *  @param R_time:红灯持续时间
 *         Y_time:黄灯持续时间
 *         G_time:绿灯持续时间
 *  @return 没有返回值
 */
    void setRYGLightTime(uint8_t R_time, uint8_t Y_time, uint8_t G_time);

/*!
 *  @brief 发送设置到MCU
 *  @param NULL
 *  @return 没有返回值
 */
    void sendMessageToMCU();

/*!
 *  @brief 获取交通灯此时的状态
 *  @param light:设置想要检测的灯
 *  @return true:  亮
 *          false：灭
 */
    bool ifLightIsOn(uint8_t light);

/*!
 *  @brief 修改8S默认的红黄绿三色灯的持续时间
 *  @param  R_time:红灯持续时间
 *         Y_time:黄灯持续时间
 *         G_time:绿灯持续时间
 *  @return NULL
 */
    void changeDefaultRYGTime(uint8_t R_time, uint8_t Y_time, uint8_t G_time);

  protected: 
/*!
 *  @brief 向传感器的指定寄存器写入数据
 *  @param Reg ：需要写入的寄存器地址
 *         Data：等待写入寄存器的数据
 *         len ：等待写入的数据的长度
 *  @return 没有返回值
 */
    virtual void writeReg(uint8_t Reg, void *Data, uint8_t len) = 0;

/*!
 *  @brief 从指定传感器中获取指定长度的数据
 *  @param INITIATIVE：传感器主动上报
 *         PASSIVITY ：主控发送请求，传感器才能上报数据
 *  @return status  ：init status
 *          true is ：init success
 *          false is：init error
 */
    virtual int16_t readReg(uint8_t Reg, uint8_t *Data, uint8_t len) = 0;  
  private:
    uint8_t _begin_hour;
    uint8_t _begin_minute;
    uint8_t _begin_second;
    uint8_t _R_time;
    uint8_t _Y_time;
    uint8_t _G_time;
};

class DFRobot_TRAFFICLIGHT_I2C : public DFRobot_TRAFFICLIGHT
{
  public:
    DFRobot_TRAFFICLIGHT_I2C(TwoWire *pWire = &Wire, uint8_t addr = 0x57);
    ~DFRobot_TRAFFICLIGHT_I2C(){};
    bool begin();
  protected:
    void writeReg(uint8_t Reg, void *Data, uint8_t len);
    int16_t readReg(uint8_t Reg, uint8_t *Data, uint8_t len);
  private:
    TwoWire *_pWire;
    uint8_t _I2C_addr;
};
#endif