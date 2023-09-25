#pragma once

#include <stdint.h>

struct mac_address {
  uint8_t bytes[6];
} __attribute((packed));

static_assert(sizeof(mac_address) == 6);

enum class frame_control_type : uint8_t {
  management = 0b00,
  control = 0b01,
  data = 0b10,
  extension = 0b11,
};

struct my_frame_control {
  uint8_t protocol_version : 2;
  frame_control_type type : 2;
  uint8_t subtype : 4;
  uint16_t unknown : 8;
} __attribute((packed));

static_assert(sizeof(my_frame_control) == 2);

struct mac_frame {
  my_frame_control frame_control;
  uint16_t duration;
  mac_address receiver_address;
  mac_address transmitter_address;
  mac_address destination_address;
  mac_address source_address;
} __attribute((packed));

void print_mac_address(mac_address &address);

void print_frame_control_type(my_frame_control &frame_control);

constexpr mac_address create_mac_address(uint8_t b1, uint8_t b2, uint8_t b3,
                                         uint8_t b4, uint8_t b5, uint8_t b6) {
  mac_address address;
  address.bytes[0] = b1;
  address.bytes[1] = b2;
  address.bytes[2] = b3;
  address.bytes[3] = b4;
  address.bytes[4] = b5;
  address.bytes[5] = b6;

  return address;
}
