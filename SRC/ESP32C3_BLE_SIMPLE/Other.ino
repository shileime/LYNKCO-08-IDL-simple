
// ===================== 工具函数 =====================
String bytesToHex(const uint8_t *data, size_t len) {
  String s;
  for (size_t i = 0; i < len; i++) {
    if (data[i] < 0x10) s += "0";
    s += String(data[i], HEX);
  }
  s.toUpperCase();
  return s;
}

String bytesToStr(const uint8_t *data, size_t len) {
  String s;
  for (size_t i = 0; i < len; i++) {
    char c = (char)data[i];
    if (c >= 32 && c <= 126) s += c;
    else s += ".";
  }
  return s;
}
String trimCopy(String s) {
  s.trim();
  return s;
}
bool splitKeyValue(const String &line, String &key, String &value) {
  String s = line;
  s.trim();
  if (s.length() == 0) return false;
  if (s.startsWith("#")) return false;
  int eq = s.indexOf('=');
  if (eq < 0) return false;
  key = trimCopy(s.substring(0, eq));
  value = trimCopy(s.substring(eq + 1));
  return key.length() > 0;
}

String boolToText(bool v) {
  return v ? "true" : "false";
}
bool textToBool(const String &text, bool defaultValue = false) {
  String s = text;
  s.toLowerCase();
  s.trim();

  if (s == "1" || s == "true" || s == "yes" || s == "on") return true;
  if (s == "0" || s == "false" || s == "no" || s == "off") return false;
  return defaultValue;
}
String readFileToString(const char *path) {
  File file = LittleFS.open(path, FILE_READ);
  if (!file) return "";
  String content;
  while (file.available()) {
    content += char(file.read());
  }
  file.close();
  return content;
}

bool writeStringToFile(const char *path, const String &content) {
  File file = LittleFS.open(path, FILE_WRITE);
  if (!file) return false;

  size_t written = file.print(content);
  file.close();
  return written == content.length();
}
void printFile(const char *path) {
  File file = LittleFS.open(path, FILE_READ);
  if (!file) {
    Serial.printf("打开失败: %s\n", path);
    return;
  }

  Serial.printf("---- %s ----\n", path);
  while (file.available()) {
    Serial.write(file.read());
  }
  Serial.println("\n----------------");
  file.close();
}
int readConfigInt(const char *key, int defaultValue) {
  String v = readConfigString(key, String(defaultValue));
  return v.toInt();
}

float readConfigFloat(const char *key, float defaultValue) {
  String v = readConfigString(key, String(defaultValue, 2));
  return v.toFloat();
}

bool readConfigBool(const char *key, bool defaultValue) {
  String v = readConfigString(key, boolToText(defaultValue));
  return textToBool(v, defaultValue);
}

bool writeConfigInt(const char *key, int value) {
  return writeConfigValue(key, String(value));
}

bool writeConfigFloat(const char *key, float value, int digits = 2) {
  return writeConfigValue(key, String(value, digits));
}

bool writeConfigBool(const char *key, bool value) {
  return writeConfigValue(key, boolToText(value));
}

bool writeConfigString(const char *key, const String &value) {
  return writeConfigValue(key, value);
}
