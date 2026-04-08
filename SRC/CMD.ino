String inputBuffer = "";               // 串口接收缓冲区
unsigned long lastCharTime = 0;        // 最后一次收到字符的时间
const unsigned long TIMEOUT_MS = 100;  // 无新字符时，等待 100ms 后处理缓冲区数据
void Serial_cmd() {
  // 非阻塞读取串口数据
  while (Serial.available()) {
    char c = Serial.read();
    inputBuffer += c;
    lastCharTime = millis();  // 更新最后一次收到字符的时间
    // 如果遇到换行符或回车符，立即处理当前缓冲区
    if (c == '\n' || c == '\r') {
      parseCommand(inputBuffer);
      inputBuffer = "";  // 清空缓冲区
      lastCharTime = 0;  // 重置时间标记
    }
  }
  // 超时处理：如果缓冲区有内容且长时间未收到新字符，则当作完整指令处理
  if (inputBuffer.length() > 0 && lastCharTime != 0 && (millis() - lastCharTime > TIMEOUT_MS)) {
    parseCommand(inputBuffer);
    inputBuffer = "";
    lastCharTime = 0;
  }
}
// 解析并执行命令
bool parseCommand(String cmd) {
  cmd.trim();  // 去掉首尾空白（包括 \r, \n, 空格等）
  if (cmd.length() == 0) return false;
  String lowerCmd = cmd;
  lowerCmd.toLowerCase();  // 转为小写，便于不敏感比较

  // 处理重启指令
  if (lowerCmd == "format") {
    Serial.println("格式化中...");
    delay(100);
    LittleFS_format();
    Serial.println("Restarting ...");
    delay(100);
    ESP.restart();
    return true;
  }
  if (lowerCmd == "restart") {
    Serial.println("Restarting ...");
    delay(100);
    ESP.restart();
    return true;
  }
  if (lowerCmd == "open") {
    openIDL();
    return true;
  }
  if (lowerCmd == "auto") {
    openIDL_auto();
    return true;
  }
  if (lowerCmd == "close") {
    setRelay(0);
    return true;
  }
  // 处理键值对指令（必须包含等号）
  int eqPos = lowerCmd.indexOf('=');
  if (eqPos == -1) {
    Serial.print("指令不识别: ");
    Serial.println(cmd);
    return false;
  }
  // 分割键和值，并去除两端空格
  String key = lowerCmd.substring(0, eqPos);
  String value = cmd.substring(eqPos + 1);
  key.trim();
  value.trim();
  // 根据键名执行不同操作
  if (key == "activate") {
    String lowerVal = value;
    lowerVal.toLowerCase();
    if (lowerVal == "true" || lowerVal == "1" || lowerVal == "on" || lowerVal == "yes") {
      activate_state(true);
    } else if (lowerVal == "false" || lowerVal == "0" || lowerVal == "off" || lowerVal == "no") {
      activate_state(false);
    } else {
      Serial.print("不识别的值: ");
      Serial.println(value);
      return false;
    }
  } else if (key == "timeout") {
    // 检查是否为纯数字（允许正数）
    bool isNumber = true;
    for (char c : value) {
      if (!isdigit(c)) {
        isNumber = false;
        break;
      }
    }
    if (!isNumber || value.length() == 0) {
      Serial.print("不识别的值: ");
      Serial.println(value);
      return false;
    }
    unsigned long timeout = value.toInt();
    set_IDL_timeOut(timeout);

  } else if (key == "idl") {
    IDL_HEX_COMMAND(value.toInt());
  } else {
    Serial.print("不识别的指令: ");
    Serial.println(key);
    return false;
  }
  return true;
}
