#include <stdint.h>

//
// SERIALIZATION
//
void encode_uint16(uint8_t *dest, uint16_t value);
void encode_uint32(uint8_t *dest, uint32_t value);

uint16_t decode_uint16(const uint8_t *src);
uint32_t decode_uint32(const uint8_t *src);

//
// PACKET TYPES
//
typedef uint8_t pkp_type;

const pkp_type PKP_TYPE_KEY_UP = 0x00;
const pkp_type PKP_TYPE_KEY_DOWN = 0x01;
const pkp_type PKP_TYPE_CHARACTERS = 0x02;
const pkp_type PKP_TYPE_WINKEYER_COMMAND = 0x03;
const pkp_type PKP_TYPE_WINKEYER_STATUS = 0x04;
const pkp_type PKP_TYPE_PING = 0x05;
const pkp_type PKP_TYPE_PONG = 0x06;
const pkp_type PKP_TYPE_MISSING = 0x07;
const pkp_type PKP_TYPE_LATE = 0x08;

//
// HEADER
//
typedef struct
{
  uint8_t header_size;
  uint8_t payload_size;
  pkp_type payload_type;
  uint8_t sequence_number;
  uint8_t address;
} pkp_header;

const uint8_t PKP_HEADER_SIZE =
    sizeof(uint8_t) +  // header size
    sizeof(uint8_t) +  // payload size
    sizeof(pkp_type) + // payload type
    sizeof(uint8_t) +  // sequence number
    sizeof(uint8_t);   // address
;

const uint8_t PKP_MAX_PAYLOAD_SIZE = 255 - PKP_HEADER_SIZE;

//
// KEY UP
//

const int PKP_KEY_UP_PAYLOAD_SIZE =
    sizeof(uint8_t) + // channel
    sizeof(uint32_t); // timestamp

typedef struct
{
  pkp_header header;
  uint8_t channel;
  uint32_t timestamp;
} pkp_key_up_packet;

//
// KEY DOWN
//
const int PKP_KEY_DOWN_PAYLOAD_SIZE =
    sizeof(uint8_t) + // channel
    sizeof(uint32_t); // timestamp

typedef struct
{
  pkp_header header;
  uint8_t channel;
  uint32_t timestamp;
} pkp_key_down_packet;

//
// CHARACTERS
//
const int PKP_CHARACTERS_BASE_PAYLOAD_SIZE =
    sizeof(uint8_t) + // channel
    sizeof(uint8_t);  // size

typedef struct
{
  pkp_header header;
  uint8_t channel;
  uint8_t size;
  uint8_t characters[PKP_MAX_PAYLOAD_SIZE - 2 * sizeof(uint8_t)];
} pkp_characters_packet;

//
// WINKEYER COMMAND
//
const int PKP_WINKEYER_COMMAND_BASE_PAYLOAD_SIZE =
    sizeof(uint8_t) + // channel
    sizeof(uint8_t);  // size
typedef struct
{
  pkp_header header;
  uint8_t channel;
  uint8_t count;
  uint8_t bytes[PKP_MAX_PAYLOAD_SIZE - 2 * sizeof(uint8_t)];
} pkp_winkeyer_command_packet;

//
// WINKEYER STATUS
//
const int PKP_WINKEYER_STATUS_BASE_PAYLOAD_SIZE =
    sizeof(uint8_t) + // channel
    sizeof(uint8_t);  // size
typedef struct
{
  pkp_header header;
  uint8_t channel;
  uint8_t count;
  uint8_t bytes[PKP_MAX_PAYLOAD_SIZE - 2 * sizeof(uint8_t)];
} pkp_winkeyer_status_packet;

//
// PING
//
const int PKP_PING_PAYLOAD_SIZE =
    sizeof(uint32_t); // timestamp
typedef struct
{
  pkp_header header;
  uint32_t timestamp;
} pkp_ping_packet;

//
// PONG
//
const int PKP_PONG_PAYLOAD_SIZE =
    sizeof(uint32_t); // timestamp
typedef struct
{
  pkp_header header;
  uint32_t timestamp;
} pkp_pong_packet;

//
// MISSING
//
const int PKP_MISSING_PAYLOAD_SIZE =
    sizeof(uint8_t); // sequence number
typedef struct
{
  pkp_header header;
  uint8_t sequence_number;
} pkp_missing_packet;

//
// LATE
//
const int PKP_LATE_PAYLOAD_SIZE =
    sizeof(uint8_t) + // sequence number
    sizeof(uint32_t); // lateness
typedef struct
{
  pkp_header header;
  uint8_t sequence_number;
  uint32_t lateness;
} pkp_late_packet;

//
// APPLICATION
//
const int PKP_APPLICATION_BASE_PAYLOAD_SIZE =
    sizeof(uint8_t); // size
typedef struct
{
  pkp_header header;
  uint8_t payload[PKP_MAX_PAYLOAD_SIZE];
} pkp_application_packet;

//
// DECODING
//
bool pkp_decode_header(uint8_t *buffer, int bufsize, pkp_header *header);

bool pkp_decode(pkp_header header, pkp_key_up_packet *packet, const uint8_t *buffer, int bufsize);
bool pkp_decode(pkp_header header, pkp_key_down_packet *packet, const uint8_t *buffer, int bufsize);
bool pkp_decode(pkp_header header, pkp_characters_packet *packet, const uint8_t *buffer, int bufsize);
bool pkp_decode(pkp_header header, pkp_winkeyer_command_packet *packet, const uint8_t *buffer, int bufsize);
bool pkp_decode(pkp_header header, pkp_winkeyer_status_packet *packet, const uint8_t *buffer, int bufsize);
bool pkp_decode(pkp_header header, pkp_ping_packet *packet, const uint8_t *buffer, int bufsize);
bool pkp_decode(pkp_header header, pkp_pong_packet *packet, const uint8_t *buffer, int bufsize);
bool pkp_decode(pkp_header header, pkp_missing_packet *packet, const uint8_t *buffer, int bufsize);
bool pkp_decode(pkp_header header, pkp_late_packet *packet, const uint8_t *buffer, int bufsize);

//
// ENCODING
//
bool pkp_encode(pkp_header header, uint8_t *buffer, int bufsize);
bool pkp_encode(const pkp_key_up_packet *packet, uint8_t *buffer, int bufsize);
bool pkp_encode(const pkp_key_down_packet *packet, uint8_t *buffer, int bufsize);
bool pkp_encode(const pkp_characters_packet *packet, uint8_t *buffer, int bufsize);
bool pkp_encode(const pkp_winkeyer_command_packet *packet, uint8_t *buffer, int bufsize);
bool pkp_encode(const pkp_winkeyer_status_packet *packet, uint8_t *buffer, int bufsize);
bool pkp_encode(const pkp_ping_packet *packet, uint8_t *buffer, int bufsize);
bool pkp_encode(const pkp_pong_packet *packet, uint8_t *buffer, int bufsize);
bool pkp_encode(const pkp_missing_packet *packet, uint8_t *buffer, int bufsize);
bool pkp_encode(const pkp_late_packet *packet, uint8_t *buffer, int bufsize);