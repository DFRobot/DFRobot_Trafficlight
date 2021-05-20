# DFRobot_Trafficlight
DFRobot's GasSensor

## DFRobot_Trafficlight Library for Arduino
---------------------------------------------------------



## Table of Contents

* [Installation](#installation)
* [Methods](#methods)
* [Compatibility](#compatibility)
* [History](#history)
* [Credits](#credits)

<snippet>
<content>

## Installation

To use this library download the zip file, uncompress it to a folder named Installation.
Download the zip file first to use this library and uncompress it to a folder named Installation.

## Methods

```C++
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
 *  @brief 更新MCU的时间
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
 *  @param NULL
 *  @return 哪个灯此时处于亮的状态
 */
    String getWhitchLightIsOn();
  protected: 
    sProtocol_t pack(uint8_t *pBuf, uint8_t len);
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
```
## Compatibility

MCU                | Work Well | Work Wrong | Untested  | Remarks
------------------ | :----------: | :----------: | :---------: | -----
FireBeetle-ESP32  |      √       |             |            | 
FireBeetle-ESP8266|      √       |              |             | 
Mega2560  |      √       |             |            | 
Arduino uno |       √      |             |            | 
Leonardo  |      √       |              |             | 




## History

- 19,05, 2021 - Version 0.2 released.


## Credits

Written by PengKaixing(kaixing.peng@dfrobot.com), 2021. (Welcome to our [website](https://www.dfrobot.com/))