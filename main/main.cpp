#include "main.h"

#include <inttypes.h>
#include "sdkconfig.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <vector>

constexpr const char* letters[] = {
".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", // A-I
".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", // J-R
"...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.." // S-Z
};

extern "C" void app_main(void)
{
  std::vector<char> message;
  message.push_back('S');
  message.push_back('O');
  message.push_back('S');
  message.push_back('\0');

  gpio_reset_pin(blink_port);
  gpio_set_direction(blink_port, GPIO_MODE_OUTPUT);

  while (true) {
    flash_morse(message.data());
  }
}

void flash_morse(const char* message) {
  for (int i = 0; message[i] != 0; i++) {
    auto morse_snippet = letter_to_morse(message[i]);
    for (int j = 0; morse_snippet[j]; j++) {
      flash_dot_or_dash(morse_snippet[j]);
    }
    flash_dot_or_dash(',');
  }
  flash_dot_or_dash(' ');
}

const char* letter_to_morse(char c) {
  if (c >= 'a' && c <= 'z') {
    return letters[c - 'a'];
  }

  return letters[c - 'A'];
}

void flash_dot_or_dash(char c) {
  switch (c) {
    case ' ':
      delay(space_delay);
    break;
    case ',':
      delay(shortspace_delay);
    break;
    case '.':
    case '-':
      gpio_set_level(blink_port, 1);
      delay(c == '.' ? dot_delay : dash_delay);
      gpio_set_level(blink_port, 0);
      delay(300);
    break;
  }
}

void delay(uint32_t milis) {
  vTaskDelay(milis / portTICK_PERIOD_MS);
}
