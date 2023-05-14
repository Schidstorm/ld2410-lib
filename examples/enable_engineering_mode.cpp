// #define LD2410_NO_ARDUINO // you can remove the Arduino.h dependency. In that case you must implement a CustomReader and CustomWriter yourself.

#include "ld2410.h"

using namespace ld2410;

// Create a reader from Arduinos Serial
// StreamReader r{Serial1}; // use this if it is supported
StreamReader r{};

// PacketReader packet_reader(CustomReader{}); // use this if you want to implement a reader yourself. CustomReader must implement reader_t (ld2410_reader.h)
PacketReader packet_reader(r);

// Create a writer from Arduinos Serial
// StreamWriter w{Serial1}; // use this if it is supported
StreamWriter w{};

// PacketWriter packet_writer(CustomWriter{}); // use this if you want to implement a writer yourself. CustomWriter must implement writerr_t (ld2410_writer.h)
PacketWriter packet_writer(w);

// Create a helper class to send commands and wait for answers
Commander commander(&packet_reader, &packet_writer);

void setup(void) {
    Serial.begin(256000);

    // wait for the first ld2410 packet
    while (!packet_reader.read().has_value());

    // each method sends a packet and waits for the correct answer.
    commander.enable_configuration();
    commander.enable_engineering_mode();
    commander.end_configuration();
}


void loop(void) {
  auto packet = packet_reader.read();

  if (packet.has_value()) {
    // print summary of packet
    Serial.println(packet->to_string().c_str());

    // print packet data as hex string
    Serial.println(packet->data_to_hexstring().c_str());
  }
}

