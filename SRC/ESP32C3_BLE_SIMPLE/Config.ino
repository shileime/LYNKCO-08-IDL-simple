void Config() {
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_RELAY, OUTPUT);
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  setRelay(false);
}

// ===================== 按键切换 =====================
static bool lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;

void handleButton() {
  bool reading = digitalRead(PIN_BUTTON);
  if ((millis() - lastDebounceTime) > 100) {
    // 如果按钮状态与上次不同
    if (reading != lastButtonState) {
      // 重置去抖动时间
      lastDebounceTime = millis();
      // 更新最后按钮状态
      lastButtonState = reading;
      // 如果按钮被按下（内部上拉，所以读到LOW是按下）
      if (reading == LOW) {
        // 切换LED状态
        debugMode = !debugMode;
        Serial.print("Debug模式: ");
        Serial.println(debugMode ? "开启" : "关闭");
      }
    }
  }
}