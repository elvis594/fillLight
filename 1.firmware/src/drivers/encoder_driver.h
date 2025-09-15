#pragma once

#include <stdint.h>

void encoder_init();
void encoder_update();

// Returns incremental deltas since last call (can be negative)
int16_t encoder_get_brightness_delta();
int16_t encoder_get_cct_delta();

