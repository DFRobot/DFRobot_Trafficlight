/*!
  * @file  getstatus.ino
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
*/

#include "DFRobot_Trafficlight.h"
#define I2C_ADDRESS    0x54
DFRobot_TRAFFICLIGHT_I2C TRAFFICLIGHT(&Wire ,I2C_ADDRESS);
String active_light = ""; 
void setup() {
  Serial.begin(115200);
  while(false==TRAFFICLIGHT.begin())
  {
    Serial.println("i2c begin fail,please chack connect!");
    delay(1000);
  }
  Serial.println("i2c begin success");

/*!
 *  @brief 修改交通灯默认持续值
 */
  TRAFFICLIGHT.changeDefaultRYGTime(/*红灯持续时间*/ 11, /*黄灯持续时间*/ 3, /*绿灯持续时间*/ 11);

  /*!
 *  @brief 设置从0时1分0开始，持续亮红灯
 */
  TRAFFICLIGHT.clearSchedule();
  TRAFFICLIGHT.setBeginTime(/*时*/0,/*分*/1,/*秒*/0);
  TRAFFICLIGHT.setRYGLightTime(/*红灯持续时间*/ LIGHT_INFINITY_TIME, /*黄灯持续时间*/ 0, /*绿灯持续时间*/ 0);
  TRAFFICLIGHT.sendMessageToMCU();
  
/*!
 *  @brief 更新MCU时间
 */  
  TRAFFICLIGHT.updateModuleTime(0,0,0);
}

void loop() {
  if (true == TRAFFICLIGHT.IfLightIsOn(RED_LIGHT))
  {
    Serial.println("light is on!");
  }else{
    Serial.println("light is off!");
  }
  delay(1000);
}
