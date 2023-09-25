#include "main.h"
#include "wifi.h"

#include "sdkconfig.h"
#include <esp_event.h>
#include <esp_wifi.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <inttypes.h>
#include <string.h>

void packet_callback(void *buf, wifi_promiscuous_pkt_type_t type) {
  auto *pkt = (wifi_promiscuous_pkt_t *)buf;
  auto *rx_ctrl = &pkt->rx_ctrl;
  auto *frame = (mac_frame *)pkt->payload;
  print_frame_control_type(frame->frame_control);

  printf("rssi: %d\n"
         "channel: %u\n"
         "sig_len: %u\n",
         rx_ctrl->rssi, rx_ctrl->channel, rx_ctrl->sig_len);

  printf("duration: %u\n", frame->duration);

  printf("receiver: ");
  print_mac_address(frame->receiver_address);

  printf("transmitter: ");
  print_mac_address(frame->transmitter_address);

  printf("destination: ");
  print_mac_address(frame->destination_address);

  printf("source: ");
  print_mac_address(frame->source_address);

  printf("\n\n\n");
}

extern "C" void app_main(void) {
  esp_netif_init();
  esp_event_loop_create_default();
  auto *sta_netif = esp_netif_create_default_wifi_sta();

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  esp_wifi_init(&cfg);

  esp_wifi_set_mode(WIFI_MODE_STA);
  esp_wifi_start();

  esp_wifi_set_promiscuous(true);
  esp_wifi_set_promiscuous_rx_cb(packet_callback);
}
