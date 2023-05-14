#include "ld2410.h"

using namespace ld2410;

// Create a reader from Arduinos Serial
StreamReader r{};
PacketReader packet_reader(r);

// Create a writer from Arduinos Serial
StreamWriter w{};
PacketWriter packet_writer(w);

// Create a helper class to send commands and wait for answers
Commander commander(&packet_reader, &packet_writer);

void setup(void) {
    Serial.begin(256000);

    // wait for the first ld2410 packet
    while (!packet_reader.read().has_value());

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

