#define MAX_BYTES 100  // 根据需要调整最大字节数
#ifndef VAR
#define VAR
// 定义一个全局变量
extern bool Activate;  //是否启用小蓝灯控制
extern bool debugMode;
extern bool IDL_state;
extern uint16_t IDL_COMPANY_ID;  //制造商ID
extern uint8_t IDL_HEX_COMMANDS[10];  //预设指令
extern uint8_t IDL_STRING_COMMANDS[10];  //预设指令
extern uint32_t SCAN_TIME_MS;
struct AppConfig {     // 配置文件结构
  String deviceName;   //设备名称
  int IDL_TIMEOUT_MS;  //灯光自动关闭
  bool Activate;       //设备启用状态
  //float threshold;
};
#endif
