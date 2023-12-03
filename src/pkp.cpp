#include "pkp.h"

//
// HEADER
//

bool pkp_decode_header(uint8_t *buffer, int bufsize, pkp_header *header)
{
  if (bufsize < PKP_HEADER_SIZE)
  {
    return false;
  }

  header->header_size = buffer[0];
  header->payload_size = buffer[1];
  header->payload_type = buffer[2];
  header->sequence_number = buffer[3];
  header->address = buffer[4];

  return true;
}

bool pkp_encode(pkp_header header, uint8_t *buffer, int bufsize)
{
  if (bufsize < PKP_HEADER_SIZE)
  {
    return false;
  }
  buffer[0] = PKP_HEADER_SIZE;
  buffer[1] = header.payload_size;
  buffer[2] = header.payload_type;
  buffer[3] = header.sequence_number;
  buffer[4] = header.address;
  return true;
}

//
// ENCODE
//

bool pkp_encode(const pkp_key_up_packet *packet, uint8_t *buffer, int bufsize)
{
  if (bufsize < PKP_HEADER_SIZE + PKP_KEY_UP_PAYLOAD_SIZE)
  {
    return false;
  }
  pkp_header header = packet->header;
  header.header_size = PKP_HEADER_SIZE;
  header.payload_size = PKP_KEY_UP_PAYLOAD_SIZE;
  header.payload_type = PKP_TYPE_KEY_UP;
  if (!pkp_encode(header, buffer, bufsize))
  {
    return false;
  }

  uint8_t *payload = buffer + PKP_HEADER_SIZE;
  payload[0] = packet->channel;
  payload[1] = packet->timestamp >> 24;
  payload[2] = packet->timestamp >> 16;
  payload[3] = packet->timestamp >> 8;
  payload[4] = packet->timestamp & 0xFF;
  return true;
}

bool pkp_encode(const pkp_key_down_packet *packet, uint8_t *buffer, int bufsize)
{
  if (bufsize < PKP_HEADER_SIZE + PKP_KEY_DOWN_PAYLOAD_SIZE)
  {
    return false;
  }

  pkp_header header = packet->header;
  header.header_size = PKP_HEADER_SIZE;
  header.payload_type = PKP_TYPE_KEY_DOWN;
  header.payload_size = PKP_KEY_DOWN_PAYLOAD_SIZE;
  if (!pkp_encode(header, buffer, bufsize))
  {
    return false;
  }

  uint8_t *payload = buffer + PKP_HEADER_SIZE;
  payload[0] = packet->channel;
  payload[1] = packet->timestamp >> 24;
  payload[2] = packet->timestamp >> 16;
  payload[3] = packet->timestamp >> 8;
  payload[4] = packet->timestamp & 0xFF;
  return true;
}

//
// DECODE
//

bool pkp_decode(pkp_header header, pkp_key_up_packet *packet, const uint8_t *buffer, int bufsize)
{
  if (bufsize < header.header_size + header.payload_size)
  {
    return false;
  }

  const uint8_t *payload = buffer + header.header_size;
  packet->header = header;
  packet->channel = payload[0];
  packet->timestamp = payload[1] << 24 | payload[2] << 16 | payload[3] << 8 | payload[4];
  return true;
}

bool pkp_decode(pkp_header header, pkp_key_down_packet *packet, const uint8_t *buffer, int bufsize)
{
  if (bufsize < header.header_size + header.payload_size)
  {
    return false;
  }

  const uint8_t *payload = buffer + header.header_size;
  packet->header = header;
  packet->channel = payload[0];
  packet->timestamp = payload[1] << 24 | payload[2] << 16 | payload[3] << 8 | payload[4];
  return true;
}