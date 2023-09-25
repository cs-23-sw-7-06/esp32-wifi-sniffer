#include "wifi.h"

#include <stdio.h>

static void print_management_subtype(my_frame_control &frame_control) {
  printf("Type: Management Subtype: ");
  switch (frame_control.subtype) {
  case 0b0000:
    printf("Association Request");
    break;

  case 0b0001:
    printf("Association Response");
    break;

  case 0b0010:
    printf("Reassociation Request");
    break;

  case 0b0011:
    printf("Reassociation Response");
    break;

  case 0b0100:
    printf("Probe Request");
    break;

  case 0b0101:
    printf("Probe Response");
    break;

  case 0b0110:
    printf("Timing Advertisement");
    break;

  case 0b0111:
  case 0b1111:
    printf("Reserved");
    break;

  case 0b1000:
    printf("Beacon");
    break;

  case 0b1001:
    printf("ATIM");
    break;

  case 0b1010:
    printf("Disassociation");
    break;

  case 0b1011:
    printf("Authentication");
    break;

  case 0b1100:
    printf("Deauthentication");
    break;

  case 0b1101:
    printf("Action");
    break;

  case 0b1110:
    printf("Action No Ack");
    break;
  }

  printf("\n");
}

static void print_control_subtype(my_frame_control &frame_control) {
  printf("Type: Control Subtype: ");

  switch (frame_control.subtype) {
  case 0b0000:
  case 0b0001:
  case 0b0010:
  case 0b1111:
    printf("Reserved");
    break;

  case 0b0011:
    printf("TACK");
    break;

  case 0b0100:
    printf("Beamforming Report Poll");
    break;

  case 0b0101:
    printf("VHT NDP Announcement");
    break;

  case 0b0110:
    printf("Control Frame Extension");
    break;

  case 0b0111:
    printf("Control Wrapper");
    break;

  case 0b1000:
    printf("Block Ack Request");
    break;

  case 0b1001:
    printf("Block Ack");
    break;

  case 0b1010:
    printf("PS-Poll");
    break;

  case 0b1011:
    printf("RTS");
    break;

  case 0b1100:
    printf("CTS");
    break;

  case 0b1101:
    printf("Ack");
    break;

  case 0b1110:
    printf("CF-End");
    break;
  }

  printf("\n");
}

static void print_data_subtype(my_frame_control &frame_control) {
  printf("Type: Data Subtype: ");

  switch (frame_control.subtype) {
  case 0b0000:
    printf("Data");
    break;

  case 0b0001:
  case 0b0010:
  case 0b0011:
  case 0b0101:
  case 0b0110:
  case 0b0111:
  case 0b1101:
    printf("Reserved");
    break;

  case 0b0100:
    printf("Null");
    break;

  case 0b1000:
    printf("QoS Data");
    break;

  case 0b1001:
    printf("QoS Data +CF-Ack");
    break;

  case 0b1010:
    printf("QoS Data +CF-Poll");
    break;

  case 0b1011:
    printf("QoS Data +CF-Ack +CF-Poll");
    break;

  case 0b1100:
    printf("QoS Null");
    break;

  case 0b1110:
    printf("QoS CF-Poll");
    break;

  case 0b1111:
    printf("QoS CF-Ack +CF-Poll");
    break;
  }

  printf("\n");
}

static void print_extension_subtype(my_frame_control &frame_control) {
  printf("Type: Extension Subtype: ");

  switch (frame_control.subtype) {
  case 0b0000:
    printf("DMG Beacon");
    break;

  case 0b0001:
    printf("SIG Beacon");
    break;

  default:
    printf("Reserved");
    break;
  }

  printf("\n");
}

void print_frame_control_type(my_frame_control &frame_control) {
  switch (frame_control.type) {
  case frame_control_type::management:
    print_management_subtype(frame_control);
    break;
  case frame_control_type::control:
    print_control_subtype(frame_control);
    break;
  case frame_control_type::data:
    print_data_subtype(frame_control);
    break;
  case frame_control_type::extension:
    print_extension_subtype(frame_control);
    break;
  }
}

void print_mac_address(mac_address &address) {
  printf("%02x:%02x:%02x:%02x:%02x:%02x\n", address.bytes[0], address.bytes[1],
         address.bytes[2], address.bytes[3], address.bytes[4],
         address.bytes[5]);
}
