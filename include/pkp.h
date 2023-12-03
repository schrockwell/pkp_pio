#include <stdint.h>

typedef uint8_t pkp_type;

const pkp_type PKP_TYPE_KEY_UP = 0x00;
const pkp_type PKP_TYPE_KEY_DOWN = 0x01;

const uint8_t PKP_HEADER_SIZE =
    sizeof(uint8_t) +  // header size
    sizeof(uint8_t) +  // payload size
    sizeof(pkp_type) + // payload type
    sizeof(uint8_t) +  // sequence number
    sizeof(uint8_t);   // address
;

typedef struct
{
  uint8_t header_size;
  uint8_t payload_size;
  pkp_type payload_type;
  uint8_t sequence_number;
  uint8_t address;
} pkp_header;

const int PKP_KEY_UP_PAYLOAD_SIZE =
    sizeof(uint8_t) + // channel
    sizeof(uint32_t); // timestamp

typedef struct
{
  pkp_header header;
  uint8_t channel;
  uint32_t timestamp;
} pkp_key_up_packet;

const int PKP_KEY_DOWN_PAYLOAD_SIZE =
    sizeof(uint8_t) + // channel
    sizeof(uint32_t); // timestamp

typedef struct
{
  pkp_header header;
  uint8_t channel;
  uint32_t timestamp;
} pkp_key_down_packet;

bool pkp_decode_header(uint8_t *buffer, int bufsize, pkp_header *header);

bool pkp_decode(pkp_header header, pkp_key_up_packet *packet, const uint8_t *buffer, int bufsize);
bool pkp_decode(pkp_header header, pkp_key_down_packet *packet, const uint8_t *buffer, int bufsize);

bool pkp_encode(pkp_header header, uint8_t *buffer, int bufsize);
bool pkp_encode(const pkp_key_up_packet *packet, uint8_t *buffer, int bufsize);
bool pkp_encode(const pkp_key_down_packet *packet, uint8_t *buffer, int bufsize);