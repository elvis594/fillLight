#ifndef LIGHT_CONTROL_H
#define LIGHT_CONTROL_H

#pragma once

void light_control_init();
void light_control_update();

// 编码器版本开关：
// 在 platformio.ini 中添加 -D USE_ENCODER_VERSION 可启用

#endif // LIGHT_CONTROL_H