static NimBLEScan* pScan = nullptr;
uint32_t SCAN_TIME_MS = 0;
// ================== 广播对象 ==================
static NimBLEAdvertising* pAdvertising = nullptr;
// ===================== 扫描回调 =====================
class MyCallbacks : public NimBLEScanCallbacks {
  void onResult(const NimBLEAdvertisedDevice* dev) override {
    //Serial.println("dev");
    // Debug模式：打印所有设备
    printDevice(dev);
    // 匹配判断
    checkMatch(dev);
  }
  void onScanEnd(const NimBLEScanResults& results, int reason) override {
    // 持续扫描
    NimBLEDevice::getScan()->start(SCAN_TIME_MS, false, true);
  }
};
void BLE_config() {
  NimBLEDevice::init("");
  pScan = NimBLEDevice::getScan();
  pScan->setScanCallbacks(new MyCallbacks());
  pScan->setDuplicateFilter(false);
  pScan->setActiveScan(false);  //开启 ActiveScan 才会“主动询问设备要更多数据”
  pScan->setMaxResults(0);
  pScan->start(SCAN_TIME_MS, false, true);
  // ----- 配置广播-----
  // pAdvertising = NimBLEDevice::getAdvertising();
  // pAdvertising->setMinInterval(160);  // 100 ms （160 * 0.625ms）
  // pAdvertising->setMaxInterval(200);  // 125 ms
}

// ===================== 数据解析 =====================
void checkMatch(const NimBLEAdvertisedDevice* dev) {
  if (!dev->haveManufacturerData()) return;
  std::string mfg = dev->getManufacturerData();
  if (mfg.length() < 2) return;  // 至少需要 Company ID (2字节)

  const uint8_t* data = (uint8_t*)mfg.data();
  uint16_t companyId = data[0] | (data[1] << 8);  // 小端序组合 Company ID
  if (companyId != IDL_COMPANY_ID) return;
  //Serial.println("id匹配");
  size_t payloadLen = mfg.length() - 2;  // 实际数据长度（去掉 Company ID）
  if (!payloadLen >= 1) return;          // 数据大于等于1 字节
  uint8_t cmd = data[2];                 // 取出命令字节
  // 检查命令是否在预定义的列表中
  for (size_t i = 0; i < sizeof(IDL_HEX_COMMANDS); i++) {
    if (cmd == IDL_HEX_COMMANDS[i]) {
      IDL_HEX_COMMAND(i);
      return;
    }
  }
  //IDL_String_COMMAND(bytesToStr(data + 2, payloadLen));  //字符串指令
}


// ===================== 打印 =====================
void printDevice(const NimBLEAdvertisedDevice* dev) {
  if (!debugMode) return;
  Serial.println("---------------------");
  if (dev->haveName()) {
    Serial.print("设备名称: ");
    Serial.println(dev->getName().c_str());
  } else {
    return;
    Serial.println("设备名称: <无>");
  }

  if (dev->haveManufacturerData()) {
    std::string mfg = dev->getManufacturerData();
    const uint8_t* data = (uint8_t*)mfg.data();
    uint16_t companyId = data[0] | (data[1] << 8);
    Serial.print("制造商HEX数据: 0x");
    Serial.println(companyId, HEX);
    size_t len = mfg.length() - 2;
    Serial.print("data(hex): ");
    Serial.println(bytesToHex(data + 2, len));
    Serial.print("data(str): ");
    Serial.println(bytesToStr(data + 2, len));
  } else {
    return;
    Serial.println("制造商HEX数据: <无>");
  }
}
// ================== 发送 BLE 广播（非阻塞） ==================
/**
 * @brief  发送包含制造商数据的广播，不会阻塞正在运行的扫描
 * @param  mfg     制造商 ID（2 字节，小端序）
 * @param  payload 命令字节数组指针
 * @param  len     命令字节长度
 */
// 全局设备名称，可自定义
const char* DEVICE_NAME = "idl";

void sendBLEBroadcast(const uint8_t* payload, size_t len) {
  if (pAdvertising == nullptr) return;

  // 构建制造商数据
  std::string mfgStr(reinterpret_cast<const char*>(&IDL_COMPANY_ID), sizeof(IDL_COMPANY_ID));
  std::string payloadStr(reinterpret_cast<const char*>(payload), len);
  NimBLEAdvertisementData advData;
  advData.setManufacturerData(mfgStr + payloadStr);

  // 添加设备完整名称（也可用 setShortName 节省空间）
  advData.setName(DEVICE_NAME);  // ⬅️ 新增

  // 更新广播
  if (pAdvertising->isAdvertising()) {
    pAdvertising->stop();
  }
  pAdvertising->setAdvertisementData(advData);
  pAdvertising->start();
}