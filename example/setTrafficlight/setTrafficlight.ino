/*!
  * @file  setTrafficlight.ino
    @n i2c 地址选择，默认i2c地址为0x57，A1、A0组合成4种IIC地址
                | A1 | A0 |
                | 0  | 0  |    0x54
                | 0  | 1  |    0x55
                | 1  | 0  |    0x56
                | 1  | 1  |    0x57   default i2c address  
  * @n 实验现象：  按照设定亮灯
  * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  * @licence     The MIT License (MIT)
  * @author      PengKaixing(kaixing.peng@dfrobot.com)
  * @version     V1.0
  * @date        2021-05-17
  * @get         from https://www.dfrobot.com
  * @url         https://github.com/dfrobot/DFRobot_Trafficlight
*/

#include "DFRobot_Trafficlight.h"
#define I2C_ADDRESS    0x54
DFRobot_TRAFFICLIGHT_I2C TRAFFICLIGHT(&Wire ,I2C_ADDRESS);

void setup() {
  Serial.begin(115200);
  while(false==TRAFFICLIGHT.begin())
  {
    Serial.println("i2c begin fail,please chack connect!");
    delay(1000);
  }
  Serial.println("i2c begin success");  

/*!
 *  @brief 设置从0时0分20开始，持续亮红灯
 */
  TRAFFICLIGHT.clearSchedule();
  TRAFFICLIGHT.setBeginTime(/*时*/0,/*分*/0,/*秒*/20);
/*!
 *  @param 范围：1~99
 *         单位：秒
 */
  TRAFFICLIGHT.setRYGLightTime(/*红灯持续时间*/1,/*黄灯持续时间*/0,/*绿灯持续时间*/0);
  TRAFFICLIGHT.sendMessageToMCU();

/*!
 *  @brief 设置从0时0分40开始，持续亮黄灯
 */  
  TRAFFICLIGHT.setBeginTime(/*时*/0,/*分*/0,/*秒*/40);
/*!
 *  @param 范围：1~99
 *         单位：秒
 */
  TRAFFICLIGHT.setRYGLightTime(/*红灯持续时间*/0,/*黄灯持续时间*/1,/*绿灯持续时间*/0);
  TRAFFICLIGHT.sendMessageToMCU(); 

/*!
 *  @brief 设置从1时0分0开始，持续亮绿灯
 */  
  TRAFFICLIGHT.setBeginTime(/*时*/0,/*分*/1,/*秒*/0);
/*!
 *  @param 范围：1~99
 *         单位：秒
 */
  TRAFFICLIGHT.setRYGLightTime(/*红灯持续时间*/0,/*黄灯持续时间*/0,/*绿灯持续时间*/1);
  TRAFFICLIGHT.sendMessageToMCU();

/*!
 *  @brief 设置从0时1分20开始，红灯亮6秒，黄灯亮5秒，绿灯亮4秒
 */  
  TRAFFICLIGHT.setBeginTime(/*时*/0,/*分*/1,/*秒*/20);
/*!
 *  @param 范围：1~99
 *         单位：秒
 */
  TRAFFICLIGHT.setRYGLightTime(/*红灯持续时间*/6,/*黄灯持续时间*/5,/*绿灯持续时间*/4);
  TRAFFICLIGHT.sendMessageToMCU();
  
/*!
 *  @brief 更新MCU时间
 */  
  TRAFFICLIGHT.updateModuleTime(0,0,0);
}

void loop() {
/*!
 *  @brief 打印时间
 */
  TRAFFICLIGHT.getNowTime();
  Serial.print(TRAFFICLIGHT._NowTime.hour);
  Serial.print(":");
  Serial.print(TRAFFICLIGHT._NowTime.minute);
  Serial.print(":");
  Serial.println(TRAFFICLIGHT._NowTime.second);
  delay(1000);
}
