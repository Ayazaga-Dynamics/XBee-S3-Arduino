#ifndef AYAZAGA_XBEE_H
#define AYAZAGA_XBEE_H

#include <Arduino.h>
#include <SoftwareSerial.h>

/**
 * Default XBEE Package Structure
 * start_delimeter: 1 byte
 * package_len: 2 bytes
 * frame_type: 1 byte
 * package_id: 1 byte
 * destination: 8 bytes
 * _16bit_destination: 2 bytes
 * broadcast_radius: 1 byte
 * options: 1 byte
 * payload: <N> bytes    == change this parameter for your payload size (max 100 bytes) ==
 * checksum: 1 byte
 */

// XBEE Package Positions
#define START_DELIMETER 0x00
#define PACKAGE_LEN_FIRST 0x01
#define PACKAGE_LEN_SECOND 0x02
#define FRAME_TYPE 0x03
#define PACKAGE_ID 0x04
#define DST_ADDR_FIRST 0x05
#define DST_ADDR_SECOND 0x06
#define DST_ADDR_THIRD 0x07
#define DST_ADDR_FOURTH 0x08
#define DST_ADDR_FIFTH 0x09
#define DST_ADDR_SIXTH 0x0A
#define DST_ADDR_SEVENTH 0x0B
#define DST_ADDR_EIGHTH 0x0C
#define _16BIT_DST_ADDR_FIRST 0x0D
#define _16BIT_DST_ADDR_SECOND 0x0E
#define BROADCAST_RADIUS 0x0F
#define OPTIONS 0x10
#define PAYLOAD_START 0x11

class XBEE
{
private:
    uint8_t rx_pin;
    uint8_t tx_pin;
    uint32_t baud_rate;

    uint8_t payload_size;
    byte *payload;

    SoftwareSerial *serial;

    static const uint8_t BASE_FRAME_SIZE = 0x0E;
    static const uint8_t NOT_INITIALIZED = 0xFF;

    // private functions
    void init_payload();
    byte calculate_checksum();

public:
    XBEE(uint8_t, uint8_t, uint8_t, uint32_t);
    ~XBEE();

    void start();
    void restart();
    void broadcast(String);
    void send_message(String, byte *);
    void message_to_coordinator(String);
};

#endif