#include <Arduino.h>
#include <NimBLEDevice.h>
#include "var.h"
static String ver="s.1.0.0";
// ===================== 引脚定义 =====================
static constexpr uint8_t PIN_LED = 3;
static constexpr uint8_t PIN_BUTTON = 9;
static constexpr uint8_t PIN_RELAY = 7;

// ===================== 状态 =====================
bool debugMode = false;

void setup() {
  Config();          //初始化IO
  Serial.begin(115200);
  delay(3000);
  Serial.println("系统初始化...");
  LittleFS_begin();  //初始化文件系统
  BLE_config();      //初始化BLE
  Serial.println("系统已启动.");
}

void loop() {
  handleButton();  //按钮检查
  IDLtimeOut();   //小蓝灯超时关闭
  Serial_cmd();    //串口指令识别
}
