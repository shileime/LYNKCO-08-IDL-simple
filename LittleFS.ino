#include "LittleFS.h"
#define FORMAT_LITTLEFS_IF_FAILED true
static const char *CONFIG_PATH = "/config.ini";

AppConfig cfg;

void LittleFS_begin() {
  Serial.println("初始化文件系统");
  if (!LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED)) {
    Serial.println("LittleFS 挂载失败");
    while (true) {
      delay(1000);
    }
  }
  Serial.println("LittleFS 挂载成功");
  if (!createConfigIfMissing()) {
    Serial.println("创建默认配置失败");
    while (true) {
      delay(1000);
    }
  }
  loadConfig(cfg);
  Serial.println("当前加载的配置：");
  Serial.println(cfg.deviceName);
  Serial.print("设备启用状态：");
  Serial.println(Activate);
  Serial.print("超时配置：");
  Serial.println(IDL_TIMEOUT);
  //Serial.println(cfg.debug ? "true" : "false");
  //Serial.println(cfg.threshold, 2);
  //demoReadWrite();
}
void LittleFS_format() {  //格式化
  LittleFS.begin();
  LittleFS.format();
  LittleFS.end();
}
// ---------------------------
// 默认配置
// ---------------------------
String defaultConfigText() {
  String s;
  s += "# ESP32 LittleFS config file\n";
  s += "# 格式: key=value\n";
  s += "\n";
  s += "deviceName=领克08小蓝灯控制器-简版\n";
  s += "Activate=true\n";
  s += "IDL_TIMEOUT_MS=3000\n";
  // s += "debug=false\n";
  // s += "threshold=23.50\n";
  return s;
}

bool createConfigIfMissing() {
  if (LittleFS.exists(CONFIG_PATH)) {
    return true;
  }
  Serial.println("配置文件不存在，正在创建默认配置...");
  return writeStringToFile(CONFIG_PATH, defaultConfigText());
}

// ---------------------------
// 通用读取
// ---------------------------
String readConfigString(const char *key, const String &defaultValue) {
  File file = LittleFS.open(CONFIG_PATH, FILE_READ);
  if (!file) return defaultValue;
  while (file.available()) {
    String line = file.readStringUntil('\n');
    line.trim();

    String k, v;
    if (splitKeyValue(line, k, v) && k == key) {
      file.close();
      return v;
    }
  }

  file.close();
  return defaultValue;
}

// ---------------------------
// 通用写入：更新单个 key
// ---------------------------
bool writeConfigValue(const char *key, const String &newValue) {
  File file = LittleFS.open(CONFIG_PATH, FILE_READ);
  String out;
  bool found = false;
  if (file) {
    while (file.available()) {
      String line = file.readStringUntil('\n');
      String raw = line;
      raw.trim();
      String k, v;
      if (splitKeyValue(raw, k, v) && k == key) {
        out += String(key) + "=" + newValue + "\n";
        found = true;
      } else {
        out += line;
        if (!line.endsWith("\n")) out += "\n";
      }
    }
    file.close();
  }
  if (!found) {
    out += String(key) + "=" + newValue + "\n";
  }
  return writeStringToFile(CONFIG_PATH, out);
}

// ---------------------------
// 载入/保存整份配置
// ---------------------------
bool loadConfig(AppConfig &c) {
  c.deviceName = readConfigString("deviceName", "领克08小蓝灯控制器-简版");  //右侧是没读取到参数使用的默认值
  c.Activate = readConfigBool("Activate", false);
  Activate = c.Activate;
  c.IDL_TIMEOUT_MS = readConfigInt("IDL_TIMEOUT_MS", 2000);
  IDL_TIMEOUT = c.IDL_TIMEOUT_MS;
  //c.threshold = readConfigFloat("threshold", 23.50f);
  return true;
}

bool saveConfig(const AppConfig &c) {
  String out;
  out += String("deviceName=") + c.deviceName + "\n";
  out += String("IDL_TIMEOUT_MS=") + c.IDL_TIMEOUT_MS + "\n";
  out += String("Activate=") + boolToText(c.Activate) + "\n";
  // out += String("threshold=") + String(c.threshold, 2) + "\n";
  return writeStringToFile(CONFIG_PATH, out);
}

// ---------------------------
// 示例：读取、修改、保存
// ---------------------------
/*
void demoReadWrite() {
  Serial.println("===== 读取示例 =====");
  Serial.print("deviceName = ");
  Serial.println(readConfigString("deviceName", "unknown"));
  Serial.print("sampleIntervalMs = ");
  Serial.println(readConfigInt("sampleIntervalMs", 0));
  Serial.print("debug = ");
  Serial.println(readConfigBool("debug", false) ? "true" : "false");
  Serial.print("threshold = ");
  Serial.println(readConfigFloat("threshold", 0.0f), 2);
  Serial.println("===== 单项写入示例 =====");
  writeConfigString("deviceName", "ESP32-Lab");
  writeConfigInt("sampleIntervalMs", 2000);
  writeConfigBool("debug", true);
  writeConfigFloat("threshold", 36.75f, 2);
  Serial.println("===== 整体写入示例 =====");
  cfg.deviceName = "ESP32-Pro";
  cfg.sampleIntervalMs = 500;
  cfg.debug = false;
  cfg.threshold = 18.25f;
  saveConfig(cfg);
  Serial.println("更新后的配置文件内容：");
  printFile(CONFIG_PATH);
}
*/
