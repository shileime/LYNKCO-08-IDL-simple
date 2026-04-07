//IDL=Intelligent driving light/智能驾驶灯
bool Activate = true;  //是否启用控制
bool IDL_state = false;
bool IDL_autoClose = false;   //启用自动关闭
uint32_t IDL_TIMEOUT = 3000;  //数据超时关闭
// =================== 预设数据匹配 =====================
uint16_t IDL_COMPANY_ID = 0x2817;  //制造商ID

uint8_t IDL_HEX_COMMANDS[] = {
  //预设指令
  0x00,  //关闭(不控制IDL)
  0x01,  //启用(响应控制)
  0x02,  //车辆制动
  0x03,  //手动开启自驾灯(不会自动超时关闭)
  0x04,  //开启自驾灯(超时自动关闭)
  0x05,  //关闭智驾灯
  0x06,  //车辆准备变道指示灯-左(智驾状态,系统准备变道的指示灯提示)
  0x07,  //车辆准备变道指示灯-右
  0x08,  //左变道(正在执行)
  0x09   //右变道
};
//IDL指令执行
void IDL_String_COMMAND(String cmd) {  //暂不启用
  Serial.print("(暂不响应字符串,请修改为HEX) cmd: ");
  Serial.println(cmd);
}
void IDL_HEX_COMMAND(uint8_t id) {
  switch (id) {
    case 0:
      activate_state(0);
      break;
    case 1:
      activate_state(1);
      break;
    case 3:
      openIDL();
      break;
    case 4:
      openIDL_auto();
      break;
    case 5:
      setRelay(0);
      break;
    default:
      Serial.print("本控制器不支持此指令: ");
      Serial.println(id);
      break;
  }
}
//打开小蓝灯
void openIDL_auto() {  //带超时自动关闭
  if (Activate) {
    setRelay(1);
    IDL_autoClose = true;  //启用超时关闭
    Serial.println("小蓝灯自动模式");
  }
}
void openIDL() {

  if (Activate) {
    IDL_autoClose = false;  //停用超时关闭
    setRelay(1);
    Serial.println("手动小蓝灯开启");
  }
}
//数据超时关闭小蓝灯
uint32_t lastMatchTime = 0;
void IDLtimeOut() {
  // 超时关闭继电器
  if (IDL_autoClose)
    if (IDL_state && millis() - lastMatchTime > IDL_TIMEOUT) {
      setRelay(false);
      Serial.println("小蓝灯已超时关闭");
    }
}
//设置小蓝灯超时时间
void set_IDL_timeOut(int time) {
  IDL_TIMEOUT = time;
  writeConfigInt("IDL_TIMEOUT_MS", IDL_TIMEOUT);
  Serial.print("小蓝灯超时已配置: ");
  Serial.println(IDL_TIMEOUT);
}
//激活状态控制
void activate_state(bool state) {
  if (state) {
    Activate = true;
    writeConfigBool("Activate", true);
    Serial.println("控制器已启用");
  } else {
    Activate = false;
    writeConfigBool("Activate", false);
    Serial.println("控制器已停止");
  }
}
void setRelay(bool state) {
  IDL_state = state;
  digitalWrite(PIN_RELAY, state ? HIGH : LOW);
  digitalWrite(PIN_LED, state ? HIGH : LOW);
  lastMatchTime = millis();
}