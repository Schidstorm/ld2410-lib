// #define LD2410_NO_ARDUINO // you can remove the Arduino.h dependency. In that case you must implement a CustomReader and CustomWriter yourself.

#include "ld2410.h"

using namespace ld2410;

// Create a reader from Arduinos Serial
// StreamReader r{Serial1}; // use this if it is supported
StreamReader r{};

// Create a writer from Arduinos Serial
// StreamWriter w{Serial1}; // use this if it is supported
StreamWriter w{};

void setup(void) {
    Serial.begin(256000);

    // wait for the first ld2410 packet (this is only for the example. It might not receive anything if EnableConfigurationCommand is active)
    while(!read_from_reader<ReportingDataFrame, EngineeringModeDataFrame>(r).has_value());
    
    EnableConfigurationCommand enable_config;
    enable_config.value(1);
    write_and_read_ack(w, r, enable_config);
    write_and_read_ack(w, r, EnableEngineeringModeCommand{});
    write_and_read_ack(w, r, EndConfigurationCommand{});
}


void loop(void) {
  auto packet = read_from_reader<ReportingDataFrame, EngineeringModeDataFrame>(r);

  if (packet.has_value()) {
    // check packet name
    if (std::holds_alternative<EngineeringModeDataFrame>(*packet)) {
      return;
    }

    // access values
    auto reporting_frame = std::get<ReportingDataFrame>(*packet);
    Serial.println(reporting_frame.detection_distance());
  }
}

