/* * Cold Chip Project v1.0 - Open Source Seed 
 * 冷芯项目 v1.0 - 开源种子版
 * 核心思想：计算不应产生温升 (Computation shouldn't be hot)
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- 1. 结构体定义 (已修复编译报错) ---
struct Photon { 
  float x, y; 
  int target; 
  bool active; // 修复：必须包含此成员以解决逻辑判定报错
};

// --- 2. 基础模拟参数 ---
Photon particles[4];
float efficiency = 99.0; // 初始效率模拟

void setup() {
  // 初始化屏幕 (使用 ESP32 默认 I2C 引脚 21, 22)
  Wire.begin(21, 22);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for(;;); // 屏幕初始化失败则停止
  }
  
  display.clearDisplay();
  
  // 初始化基础光子流
  for(int i=0; i<4; i++) {
    particles[i].x = 64;
    particles[i].y = 32;
    particles[i].target = i;
    particles[i].active = true;
  }
}

void loop() {
  display.clearDisplay();

  // --- A. 渲染冷场背景 ---
  for(int i=0; i<5; i++) {
    display.drawPixel(random(128), random(50), WHITE);
  }

  // --- B. 演示逻辑：光子避热流转 ---
  for(int i=0; i<4; i++) {
    if(particles[i].active) {
      // 模拟信号在 0.5nm 逻辑岛屿间的无损传输
      display.fillCircle(particles[i].x, particles[i].y, 2, WHITE);
      
      // 简单的随机游走，模拟基础避热路径
      particles[i].x += random(-2, 3);
      particles[i].y += random(-2, 3);
      
      // 边界锁定，防止信号溢出
      if(particles[i].x < 0 || particles[i].x > 128) particles[i].x = 64;
      if(particles[i].y < 0 || particles[i].y > 50) particles[i].y = 32;
    }
  }

  // --- C. 核心公式横向展示 (直观版) ---
  display.drawFastHLine(0, 53, 128, WHITE);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 56);
  
  // 展示我们的非耗散口号
  display.print("EFF:(Er/Et)->100% COOL"); //
  
  display.display();
  delay(30); 
}
