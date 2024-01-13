#include <string.h>

#include "pkp.h"

//
// "PRIVATE" FUNCTIONS
//

bool __encode_header(pkp_header header, pkp_type type, uint8_t payload_size, uint8_t *buffer, int bufsize)
{
  if (bufsize < PKP_HEADER_SIZE + payload_size)
  {
    return false;
  }
  header.header_size = PKP_HEADER_SIZE;
  header.payload_size = payload_size;
  header.payload_type = type;
  if (!pkp_encode(header, buffer, bufsize))
  {
    return false;
  }

  return true;
}

bool __valid_bufsize(pkp_header header, int bufsize)
{
  return bufsize >= header.header_size + header.payload_size;
}

//
// SERIALIZATION
//

void encode_uint16(uint8_t *dest, uint16_t value)
{
  dest[0] = value >> 8;
  dest[1] = value & 0xFF;
};

void encode_uint32(uint8_t *dest, uint32_t value)
{
  dest[0] = value >> 24;
  dest[1] = value >> 16;
  dest[2] = value >> 8;
  dest[3] = value & 0xFF;
};

uint16_t decode_uint16(const uint8_t *src)
{
  return (src[0] << 8) | src[1];
};

uint32_t decode_uint32(const uint8_t *src)
{
  return (src[0] << 24) | (src[1] << 16) | (src[2] << 8) | src[3];
};

//
// ENCODE
//

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

bool pkp_encode(const pkp_key_up_packet *packet, uint8_t *buffer, int bufsize)
{
  if (!__encode_header(packet->header, PKP_TYPE_KEY_UP, PKP_KEY_UP_PAYLOAD_SIZE, buffer, bufsize))
  {
    return false;
  }

  uint8_t *payload = buffer + PKP_HEADER_SIZE;
  payload[0] = packet->channel;
  encode_uint32(payload + 1, packet->timestamp);
  return true;
}

bool pkp_encode(const pkp_key_down_packet *packet, uint8_t *buffer, int bufsize)
{
  if (!__encode_header(packet->header, PKP_TYPE_KEY_DOWN, PKP_KEY_DOWN_PAYLOAD_SIZE, buffer, bufsize))
  {
    return false;
  }

  uint8_t *payload = buffer + PKP_HEADER_SIZE;
  payload[0] = packet->channel;
  encode_uint32(payload + 1, packet->timestamp);
  return true;
}

bool pkp_encode(const pkp_characters_packet *packet, uint8_t *buffer, int bufsize)
{
  if (!__encode_header(packet->header, PKP_TYPE_CHARACTERS, PKP_CHARACTERS_BASE_PAYLOAD_SIZE + packet->size, buffer, bufsize))
  {
    return false;
  }

  uint8_t *payload = buffer + PKP_HEADER_SIZE;
  payload[0] = packet->channel;
  payload[1] = packet->size;
  memcpy(payload + 2, packet->characters, packet->size);
  return true;
}

bool pkp_encode(const pkp_winkeyer_command_packet *packet, uint8_t *buffer, int bufsize)
{
  if (!__encode_header(packet->header, PKP_TYPE_WINKEYER_COMMAND, PKP_WINKEYER_COMMAND_BASE_PAYLOAD_SIZE + packet->count, buffer, bufsize))
  {
    return false;
  }

  uint8_t *payload = buffer + PKP_HEADER_SIZE;
  payload[0] = packet->channel;
  payload[1] = packet->count;
  memcpy(payload + 2, packet->bytes, packet->count);
  return true;
}

bool pkp_encode(const pkp_winkeyer_status_packet *packet, uint8_t *buffer, int bufsize)
{
  if (!__encode_header(packet->header, PKP_TYPE_WINKEYER_STATUS, PKP_WINKEYER_STATUS_BASE_PAYLOAD_SIZE + packet->count, buffer, bufsize))
  {
    return false;
  }

  uint8_t *payload = buffer + PKP_HEADER_SIZE;
  payload[0] = packet->channel;
  payload[1] = packet->count;
  memcpy(payload + 2, packet->bytes, packet->count);
  return true;
}

bool pkp_encode(const pkp_ping_packet *packet, uint8_t *buffer, int bufsize)
{
  if (!__encode_header(packet->header, PKP_TYPE_PING, PKP_PING_PAYLOAD_SIZE, buffer, bufsize))
  {
    return false;
  }

  uint8_t *payload = buffer + PKP_HEADER_SIZE;
  encode_uint32(payload, packet->timestamp);
  return true;
}

bool pkp_encode(const pkp_pong_packet *packet, uint8_t *buffer, int bufsize)
{
  if (!__encode_header(packet->header, PKP_TYPE_PONG, PKP_PONG_PAYLOAD_SIZE, buffer, bufsize))
  {
    return false;
  }

  uint8_t *payload = buffer + PKP_HEADER_SIZE;
  encode_uint32(payload, packet->timestamp);
  return true;
}

bool pkp_encode(const pkp_missing_packet *packet, uint8_t *buffer, int bufsize)
{
  if (!__encode_header(packet->header, PKP_TYPE_MISSING, PKP_MISSING_PAYLOAD_SIZE, buffer, bufsize))
  {
    return false;
  }

  uint8_t *payload = buffer + PKP_HEADER_SIZE;
  payload[0] = packet->sequence_number;
  return true;
}

bool pkp_encode(const pkp_late_packet *packet, uint8_t *buffer, int bufsize)
{
  if (!__encode_header(packet->header, PKP_TYPE_LATE, PKP_LATE_PAYLOAD_SIZE, buffer, bufsize))
  {
    return false;
  }

  uint8_t *payload = buffer + PKP_HEADER_SIZE;
  payload[0] = packet->sequence_number;
  encode_uint32(payload + 1, packet->lateness);
  return true;
}

bool pkp_encode(const pkp_application_packet *packet, uint8_t *buffer, int bufsize)
{
  pkp_header header = packet->header;
  if (!__encode_header(packet->header, packet->header.payload_type, header.payload_size, buffer, bufsize))
  {
    return false;
  }

  uint8_t *payload = buffer + PKP_HEADER_SIZE;
  memcpy(payload, packet->payload, header.payload_size);
  return true;
}

//
// DECODE
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

bool pkp_decode(pkp_header header, pkp_key_up_packet *packet, const uint8_t *buffer, int bufsize)
{
  if (!__valid_bufsize(header, bufsize))
  {
    return false;
  }

  const uint8_t *payload = buffer + header.header_size;
  packet->header = header;
  packet->channel = payload[0];
  packet->timestamp = decode_uint32(payload + 1);
  return true;
}

bool pkp_decode(pkp_header header, pkp_key_down_packet *packet, const uint8_t *buffer, int bufsize)
{
  if (!__valid_bufsize(header, bufsize))
  {
    return false;
  }

  const uint8_t *payload = buffer + header.header_size;
  packet->header = header;
  packet->channel = payload[0];
  packet->timestamp = decode_uint32(payload + 1);
  return true;
}

bool pkp_decode(pkp_header header, pkp_characters_packet *packet, const uint8_t *buffer, int bufsize)
{
  if (!__valid_bufsize(header, bufsize))
  {
    return false;
  }

  const uint8_t *payload = buffer + header.header_size;
  packet->header = header;
  packet->channel = payload[0];
  packet->size = payload[1];
  memcpy(packet->characters, payload + 2, packet->size);
  return true;
}

bool pkp_decode(pkp_header header, pkp_winkeyer_command_packet *packet, const uint8_t *buffer, int bufsize)
{
  if (!__valid_bufsize(header, bufsize))
  {
    return false;
  }

  const uint8_t *payload = buffer + header.header_size;
  packet->header = header;
  packet->channel = payload[0];
  packet->count = payload[1];
  memcpy(packet->bytes, payload + 2, packet->count);
  return true;
}

bool pkp_decode(pkp_header header, pkp_winkeyer_status_packet *packet, const uint8_t *buffer, int bufsize)
{
  if (!__valid_bufsize(header, bufsize))
  {
    return false;
  }

  const uint8_t *payload = buffer + header.header_size;
  packet->header = header;
  packet->channel = payload[0];
  packet->count = payload[1];
  memcpy(packet->bytes, payload + 2, packet->count);
  return true;
}

bool pkp_decode(pkp_header header, pkp_ping_packet *packet, const uint8_t *buffer, int bufsize)
{
  if (!__valid_bufsize(header, bufsize))
  {
    return false;
  }

  const uint8_t *payload = buffer + header.header_size;
  packet->header = header;
  packet->timestamp = decode_uint32(payload);
  return true;
}

bool pkp_decode(pkp_header header, pkp_pong_packet *packet, const uint8_t *buffer, int bufsize)
{
  if (!__valid_bufsize(header, bufsize))
  {
    return false;
  }

  const uint8_t *payload = buffer + header.header_size;
  packet->header = header;
  packet->timestamp = decode_uint32(payload);
  return true;
}

bool pkp_decode(pkp_header header, pkp_missing_packet *packet, const uint8_t *buffer, int bufsize)
{
  if (!__valid_bufsize(header, bufsize))
  {
    return false;
  }

  const uint8_t *payload = buffer + header.header_size;
  packet->header = header;
  packet->sequence_number = payload[0];
  return true;
}

bool pkp_decode(pkp_header header, pkp_late_packet *packet, const uint8_t *buffer, int bufsize)
{
  if (!__valid_bufsize(header, bufsize))
  {
    return false;
  }

  const uint8_t *payload = buffer + header.header_size;
  packet->header = header;
  packet->sequence_number = payload[0];
  packet->lateness = decode_uint32(payload + 1);
  return true;
}

bool pkp_decode(pkp_header header, pkp_application_packet *packet, const uint8_t *buffer, int bufsize)
{
  if (!__valid_bufsize(header, bufsize))
  {
    return false;
  }

  const uint8_t *payload = buffer + header.header_size;
  packet->header = header;
  memcpy(packet->payload, payload, header.payload_size);
  return true;
}
