#include "main.h"

#include <inttypes.h>
#include <string.h>
#include "sdkconfig.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_wifi.h>
#include <esp_event.h>

constexpr size_t max_wifi_records = 64;


extern "C" void app_main(void)
{
  esp_netif_init();
  esp_event_loop_create_default();
  auto* sta_netif = esp_netif_create_default_wifi_sta();

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  esp_wifi_init(&cfg);

  wifi_ap_record_t* wifi_records = new wifi_ap_record_t[max_wifi_records];
  memset(wifi_records, 0, max_wifi_records * sizeof(wifi_records[0]));


  esp_wifi_set_mode(WIFI_MODE_STA);
  esp_wifi_start();
  esp_wifi_scan_start(NULL, true);

  uint16_t num_wifi_records = max_wifi_records;

  esp_wifi_scan_get_ap_records(&num_wifi_records, wifi_records);
  printf("Found %d wifi access points\n", num_wifi_records);
  for (uint16_t i = 0; i < num_wifi_records; i++) {
    printf("SSID: %s\n", wifi_records[i].ssid);
  }

  delete[] wifi_records;

  vTaskDelay(1000 / portTICK_PERIOD_MS);

  esp_restart();
}

