#include "button_driver.h"
#include "../config.h"
#include <Arduino.h>
#include <OneButton.h>

// 使用 OneButton 库来处理按键（IO4，默认上拉，按下接地，低电平有效）
static OneButton s_btn(UV_BUTTON_PIN, true, true); // 引脚，activeLow，pullupActive
static bool current_state = false;
static unsigned long last_debug_time = 0;
static bool init_done = false;

void onClick()
{
    current_state = !current_state;
    Serial.println("*** BUTTON CLICKED! State changed to: " + String(current_state ? "ON" : "OFF") + " ***");
}

void button_init() {
    // 设定去抖时间为25ms，适合机械按键
    s_btn.setDebounceMs(25);
    // 使用新的API设置单击和长按时间
    s_btn.setClickMs(300);
    s_btn.setPressMs(700);
    s_btn.attachClick(onClick);
    
    Serial.println("*** Button driver initialized on pin " + String(UV_BUTTON_PIN) + " ***");
    init_done = true;
}

bool is_uv_button_pressed() {
    return current_state == true;
}

void button_update()
{
    s_btn.tick();
}