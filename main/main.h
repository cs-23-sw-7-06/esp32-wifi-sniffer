#pragma once

#include <inttypes.h>
#include <driver/gpio.h>


constexpr static gpio_num_t blink_port = GPIO_NUM_2;

constexpr static uint32_t space_delay = 1200;
constexpr static uint32_t shortspace_delay = 600;
constexpr static uint32_t dot_delay = 300;
constexpr static uint32_t dash_delay = 900;


void flash_morse(const char* message);
const char* letter_to_morse(char c);
void flash_dot_or_dash(char c);
void delay(uint32_t milis);
