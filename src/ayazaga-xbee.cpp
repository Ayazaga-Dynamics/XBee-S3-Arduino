#include "ayazaga-xbee.h"

XBEE::XBEE(uint8_t __rx_pin,
           uint8_t __tx_pin,
           uint8_t __payload_size = 30,
           uint32_t __baud_rate = 9600)
{
    this->rx_pin = __rx_pin;
    this->tx_pin = __tx_pin;
    this->baud_rate = __baud_rate;

    this->payload_size = __payload_size + BASE_FRAME_SIZE;
    this->payload = new byte[payload_size];
    this->serial = new SoftwareSerial(rx_pin, tx_pin);
}

XBEE::~XBEE()
{
    delete this->payload;
    delete this->serial;
}

void XBEE::start()
{
    serial->begin(baud_rate);
    init_payload();
}

void XBEE::restart()
{
    // TODO
    // restart xbee module
}

void XBEE::init_payload()
{
    payload[START_DELIMETER] = 0x7E;

    memset(payload + PACKAGE_LEN_FIRST, NOT_INITIALIZED, 2);

    payload[FRAME_TYPE] = 0x10;
    payload[PACKAGE_ID] = 0x00;

    memset(payload + DST_ADDR_FIRST, NOT_INITIALIZED, 8);

    payload[_16BIT_DST_ADDR_FIRST] = 0XFF;
    payload[_16BIT_DST_ADDR_SECOND] = 0XFE;
    payload[BROADCAST_RADIUS] = 0X00;
    payload[OPTIONS] = 0X00;
}

void XBEE::broadcast(String message)
{
    // set destination address to 0x000000000000FFFF (broadcast)
    memset(payload + DST_ADDR_FIRST, 0x00, 6);
    payload[DST_ADDR_SEVENTH] = 0xFF;
    payload[DST_ADDR_EIGHTH] = 0xFF;

    // set payload size
    payload[PACKAGE_LEN_FIRST] = (payload_size >> 8) & 0xFF;
    payload[PACKAGE_LEN_SECOND] = payload_size & 0xFF;

    // set payload message
    for (unsigned int i = 0; i < message.length(); i++)
        payload[PAYLOAD_START + i] = message.charAt(i);

    // set checksum
    byte chk = calculate_checksum();
    payload[payload_size - 1] = chk;

    // send message
    serial->write(payload, payload_size);
}

void XBEE::send_message(String message, byte *dest)
{
    // set address
    memcpy(payload + DST_ADDR_FIRST, dest, 8);

    // set payload size
    payload[PACKAGE_LEN_FIRST] = (payload_size >> 8) & 0xFF;
    payload[PACKAGE_LEN_SECOND] = payload_size & 0xFF;

    // set payload message
    for (unsigned int i = 0; i < message.length(); i++)
        payload[PAYLOAD_START + i] = message.charAt(i);

    // set checksum
    byte chk = calculate_checksum();
    payload[payload_size - 1] = chk;

    // send message
    serial->write(payload, payload_size);
}

void XBEE::message_to_coordinator(String message)
{
    // set address
    memset(payload + DST_ADDR_FIRST, 0x00, 8);

    // set payload size
    payload[PACKAGE_LEN_FIRST] = (payload_size >> 8) & 0xFF;
    payload[PACKAGE_LEN_SECOND] = payload_size & 0xFF;

    // set payload message
    for (unsigned int i = 0; i < message.length(); i++)
        payload[PAYLOAD_START + i] = message.charAt(i);

    // set checksum
    byte chk = calculate_checksum();
    payload[payload_size - 1] = chk;

    // send message
    serial->write(payload, payload_size);
}

byte XBEE::calculate_checksum()
{
    /**
     *  checksum formula: 0xFF - (sum of all bytes from frame length to end of payload)
     */

    byte sum = 0x00;

    for (int i = 3; i < payload_size - 1; ++i)
        sum += payload[i];

    return 0xFF - sum;
}
