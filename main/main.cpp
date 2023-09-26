#include "main.h"
#include "wifi.h"

#include "sdkconfig.h"
#include <driver/gpio.h>
#include <esp_event.h>
#include <esp_log.h>
#include <esp_wifi.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>
#include <inttypes.h>
#include <string.h>

constexpr static auto blink_port = (gpio_num_t)CONFIG_BLINK_GPIO_NUM;
constexpr static auto blink_speed = CONFIG_BLINK_SPEED; // ms

constexpr static auto sniffer_tag = "sniffer";

constexpr static auto queue_size = CONFIG_PACKET_QUEUE_SIZE;
constexpr static auto queue_timeout = CONFIG_PACKET_QUEUE_TIMEOUT; // ms

constexpr static auto channel_switcher_frequency =
    CONFIG_CHANNEL_SWITCHER_FREQUENCY; // ms

static TaskHandle_t blinker_task;
static QueueHandle_t packet_queue;

struct queue_item {
  wifi_pkt_rx_ctrl_t rx_ctrl;
  void *payload;
};

extern "C" void app_main(void) {
  packet_queue = xQueueCreate(queue_size, sizeof(queue_item));

  setup_blinker();

  setup_wifi();

  while (true) {
    queue_item item;

    // Block forever until we receive a packet from the queue.
    if (xQueueReceive(packet_queue, &item, portMAX_DELAY) != pdTRUE) {
      // Keep trying :)
      continue;
    }

    auto *frame = (mac_frame *)item.payload;
    print_frame_control_type(frame->frame_control);

    printf("rssi: %d\n"
           "channel: %u\n",
           item.rx_ctrl.rssi, item.rx_ctrl.channel);

    printf("receiver: ");
    print_mac_address(frame->receiver_address);

    printf("transmitter: ");
    print_mac_address(frame->transmitter_address);

    printf("\n\n\n");

    free(item.payload);
  }
}

static void packet_callback(void *buf, wifi_promiscuous_pkt_type_t type) {
  auto *pkt = (wifi_promiscuous_pkt_t *)buf;
  auto *rx_ctrl = &pkt->rx_ctrl;

  auto *new_payload = malloc(rx_ctrl->sig_len);
  memcpy(new_payload, pkt->payload, rx_ctrl->sig_len);

  queue_item item{pkt->rx_ctrl, new_payload};

  xTaskNotifyGive(blinker_task);

  if (xQueueSend(packet_queue, &item, pdMS_TO_TICKS(queue_timeout)) != pdTRUE) {
    ESP_LOGE(sniffer_tag, "queue full");
    free(new_payload);
  }
}

static void task_blinker(void *parameters) {
  while (true) {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    gpio_set_level(blink_port, 1);
    // new_packet = 0;
    vTaskDelay(blink_speed / portTICK_PERIOD_MS);
    gpio_set_level(blink_port, 0);
    vTaskDelay(blink_speed / portTICK_PERIOD_MS);
  }
}

static void task_channel_hopper(void *parameters) {
  auto channel = 0;
  while (true) {
    esp_wifi_set_channel(channel + 1, WIFI_SECOND_CHAN_NONE);
    channel = (channel + 1) % 12;
    vTaskDelay(channel_switcher_frequency / portTICK_PERIOD_MS);
  }
}

static void setup_blinker() {
  gpio_reset_pin(blink_port);
  gpio_set_direction(blink_port, GPIO_MODE_OUTPUT);

  xTaskCreate(task_blinker, "blinker", 2048, (void *)nullptr, tskIDLE_PRIORITY,
              &blinker_task);
}

static void setup_wifi() {
  esp_netif_init();
  esp_event_loop_create_default();
  auto *sta_netif = esp_netif_create_default_wifi_sta();

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  esp_wifi_init(&cfg);

  esp_wifi_set_mode(WIFI_MODE_STA);
  esp_wifi_start();

  esp_wifi_set_promiscuous(true);
  esp_wifi_set_promiscuous_rx_cb(packet_callback);

  xTaskCreate(task_channel_hopper, "channel hopper", 2048, (void *)nullptr,
              tskIDLE_PRIORITY, NULL);
}
