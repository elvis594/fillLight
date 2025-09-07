#include "button_driver.h"
#include "../config.h"
#include <Arduino.h>

// 使用中断方式检测按键状态（IO4，默认上拉，按下接地）
static volatile bool s_btn_pressed = false;     // 当前按下状态（去抖后）
static volatile uint32_t s_last_change_us = 0;  // 上次有效电平变化时间戳（us）
static volatile int s_idle_level = HIGH;        // 空闲电平，初始化时采样，用于兼容外部接法

static void IRAM_ATTR uv_button_isr() {
    uint32_t now = micros();
    // 简单去抖：10ms 窗口内忽略抖动
    if (now - s_last_change_us < 10000) {
        return;
    }
    // 读取当前电平，与空闲电平不同则认为按下
    int level = digitalRead(UV_BUTTON_PIN);
    s_btn_pressed = (level != s_idle_level);
    s_last_change_us = now;
}

void button_init() {
    // 先尝试内部上拉
    pinMode(UV_BUTTON_PIN, INPUT_PULLUP);
    delay(5);
    int level_after_pullup = digitalRead(UV_BUTTON_PIN);

    // 如果上拉后仍为LOW，推测外部为下拉接法/按钮接VCC，切换为下拉
    bool using_pulldown = false;
    if (level_after_pullup == LOW) {
        pinMode(UV_BUTTON_PIN, INPUT_PULLDOWN);
        delay(5);
        using_pulldown = true;
    }

    // 采样空闲电平
    s_idle_level = digitalRead(UV_BUTTON_PIN);
    // 让首次有效边沿不过滤
    s_last_change_us = micros() - 50000;
    // 监听电平变化（按下与释放），在ISR中更新当前状态
    attachInterrupt(digitalPinToInterrupt(UV_BUTTON_PIN), uv_button_isr, CHANGE);

    // 调试输出当前配置
    Serial.printf("BTN4 init: pin=%d, mode=%s, idle=%s\n", UV_BUTTON_PIN,
                  using_pulldown ? "INPUT_PULLDOWN" : "INPUT_PULLUP",
                  s_idle_level == HIGH ? "HIGH" : "LOW");
}

bool is_uv_button_pressed() {
    // 返回当前按键是否按下（已在ISR中完成基本去抖）
    return s_btn_pressed;
}