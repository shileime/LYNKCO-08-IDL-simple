```markdown
> 🚗 50元实现领克08智驾小蓝灯 | ESP32 + EVCC 超简方案 |
ESP32主板链接：https://item.taobao.com/item.htm?id=823042315028&mi_id=0000UEq26LaSY20KMpq517pt6S4_-dz2iyrjgV32TzdTn5Q&spm=tbpc.boughtlist.suborder_itemtitle.1.6cee2e8dCE8xpD
推荐行车灯链接：https://item.taobao.com/item.htm?id=884230283410&mi_id=0000I5rHPnDeVoxXl5bjj9Zh-QpIy3uCV29TlhAuPDZ61oQ&spm=tbpc.boughtlist.suborder_itemtitle.1.6cee2e8dCE8xpD
---
### 效果展示

![运行效果](./images/1.jpg)
![车外效果](./images/2.jpg)

### 安装位置示意

![安装位置](./images/3.jpg)

---

## 接线图
![接线图](./images/主板.jpg)

### 接线说明

- **电源输入**
  - 支持 5V ~ 24V

## 快速开始（Quick Start）

### 第一步：准备硬件

- ESP32-C3 开发板 ×1  
- 小蓝灯 ×1  
- 电源（5V~24V）  
---

### 第二步：安装开发环境
1. 安装 Arduino IDE  
2. 添加 ESP32 开发板支持  
3. 选择开发板：ESP32C3 Dev Module
4. 
---

### 第三步：烧录程序

1. 打开本项目代码  
2. 连接开发板  
3. 选择串口  
4. 点击上传  
建议修改IDL.ino中“IDL_COMPANY_ID = 0x2817;  //制造商ID”为其他ID,避免冲突（虽然概率不大）
---

### 第四步：接入车辆

1. 确认 EVCC 已正常安装运行,触发条件不同车型不一样，文档最后列举
2. BLE广播设置：制造商ID(hex)默认配置为2817，如果上方修改了代码填入修改后的ID
3. 预留数据指令：
              0x00,  //关闭(不控制IDL)
              0x01,  //启用(响应控制)
(本简版方案不支持)0x02,  //车辆制动
              0x03,  //手动开启自驾灯(不会自动超时关闭)
              0x04,  //开启自驾灯(超时自动关闭)
              0x05,  //关闭智驾灯
(本简版方案不支持)0x06,  //车辆准备变道指示灯-左(智驾状态,系统准备变道的指示灯提示)
(本简版方案不支持)0x07,  //车辆准备变道指示灯-右
(本简版方案不支持)0x08,  //左变道(正在执行)
(本简版方案不支持)0x09   //右变道
4. 将设备接入车载电源  
5. 测试智驾状态触发灯效  

---
