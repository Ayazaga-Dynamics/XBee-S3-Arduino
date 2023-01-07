#include "ayazaga-xbee.h"

#define RX_PIN 2
#define TX_PIN 3
#define BAUD_RATE 9600
#define PAYLOAD_SIZE 14

XBEE xbee(RX_PIN, TX_PIN, PAYLOAD_SIZE, BAUD_RATE);

void setup()
{
    digitalWrite(LED_BUILTIN, LOW);
    xbee.start();
}

void loop()
{
    float lat = rand();
    float lon = rand();

    String payload = String(lat, 6) + String(",") + String(lon, 6) + String(",");
    xbee.message_to_coordinator(payload);

    delay(1000);

    // xbee.broadcast(payload);
    // xbee.send_message(payload, (byte *)"00AABBCCDDEEFF00");
}
