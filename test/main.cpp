#include <iostream>
#include "pkp.h"
#include "main.h"

const int BUFFER_SIZE = 1024;

bool test_encode_header()
{
  // GIVEN
  uint8_t buffer[BUFFER_SIZE];
  pkp_header header = {
      .header_size = PKP_HEADER_SIZE,
      .payload_size = PKP_KEY_UP_PAYLOAD_SIZE,
      .payload_type = PKP_TYPE_KEY_UP,
      .sequence_number = 1,
      .address = 2};

  // WHEN
  bool success = pkp_encode(header, buffer, BUFFER_SIZE);

  // THEN
  ASSERT(success, "unsuccessful");
  ASSERT(buffer[0] == PKP_HEADER_SIZE, "header size is wrong");
  ASSERT(buffer[1] == PKP_KEY_UP_PAYLOAD_SIZE, "payload size is wrong");
  ASSERT(buffer[2] == PKP_TYPE_KEY_UP, "payload type is wrong");
  ASSERT(buffer[3] == 1, "sequence number is wrong");
  ASSERT(buffer[4] == 2, "address is wrong");

  return true;
}

bool test_decode_header()
{
  // GIVEN
  pkp_header header;
  uint8_t buffer[] = {
      5,    // header size
      123,  // payload size
      0x12, // payload type
      4,    // sequence number
      5};   // address

  // WHEN
  bool success = pkp_decode_header(buffer, 5, &header);

  // THEN
  ASSERT(success, "unsuccessful");
  ASSERT(header.header_size == 5, "header size is wrong");
  ASSERT(header.payload_size == 123, "payload size is wrong");
  ASSERT(header.payload_type == 0x12, "payload type is wrong");
  ASSERT(header.sequence_number == 4, "sequence number is wrong");
  ASSERT(header.address == 5, "address is wrong");

  return true;
}

bool test_encode_key_up()
{
  // GIVEN
  uint8_t buffer[BUFFER_SIZE];
  pkp_header header = {
      .header_size = PKP_HEADER_SIZE,
      .payload_size = PKP_KEY_UP_PAYLOAD_SIZE,
      .payload_type = PKP_TYPE_KEY_UP,
      .sequence_number = 1,
      .address = 2};

  pkp_key_up_packet packet = {
      .header = header,
      .channel = 3,
      .timestamp = 4};

  // WHEN
  bool success = pkp_encode(&packet, buffer, BUFFER_SIZE);

  // THEN
  ASSERT(success, "unsuccessful");

  return true;
}

bool test_decode_key_up()
{
  pkp_header header = {
      .header_size = PKP_HEADER_SIZE,
      .payload_size = PKP_KEY_UP_PAYLOAD_SIZE,
      .payload_type = PKP_TYPE_KEY_UP,
      .sequence_number = 4,
      .address = 5};

  u_int8_t buffer[] = {
      PKP_HEADER_SIZE,
      PKP_KEY_UP_PAYLOAD_SIZE,
      PKP_TYPE_KEY_UP,
      4,         // sequence number
      5,         // address
      6,         // channel
      0, 0, 0, 7 // timestamp
  };

  pkp_key_up_packet packet;

  // WHEN
  bool success = pkp_decode(header, &packet, buffer, sizeof(buffer));

  // THEN
  ASSERT(success, "unsuccessful");
  ASSERT(packet.channel == 6, "channel is wrong");
  ASSERT(packet.timestamp == 7, "timestamp is wrong");

  return true;
}

// TODO: test_encode_key_down
// TODO: test_decode_key_down
// TODO: test_encode_characters
// TODO: test_decode_characters
// TODO: test_encode_winkeyer_command
// TODO: test_decode_winkeyer_command
// TODO: test_encode_winkeyer_status
// TODO: test_decode_winkeyer_status
// TODO: test_encode_ping
// TODO: test_decode_ping
// TODO: test_encode_pong
// TODO: test_decode_pong
// TODO: test_encode_missing
// TODO: test_decode_missing
// TODO: test_encode_late
// TODO: test_decode_late
// TODO: test_encode_application
// TODO: test_decode_application

int main()
{
  TEST("encode header", test_encode_header());
  TEST("decode header", test_decode_header());

  TEST("encode key up", test_encode_key_up());
  TEST("decode key up", test_decode_key_up());

  return 0;
}