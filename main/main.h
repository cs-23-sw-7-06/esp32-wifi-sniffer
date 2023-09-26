#pragma once

#include <esp_wifi.h>

static void packet_callback(void *buf, wifi_promiscuous_pkt_type_t type);

static void task_blinker(void *parameters);

static void task_channel_hopper(void *parameters);

static void setup_blinker();

static void setup_wifi();
