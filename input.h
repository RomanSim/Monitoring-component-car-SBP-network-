struct entry {
  uint64_t timestmap;
  uint8_t pedal_break;
  uint8_t pedal_gas;
  uint8_t abs_active;
  uint8_t tire_preasure;
  uint16_t speedometer;
}

void inspect_speedometer_frequency(frame * frames, size_t count) {
    // Check we have only 1 speedometer frame in all frames
    size_t matches = 0;
    for (size_t i = 0; i < count; i++) {
      if (frames[i].type == FRAME_SPEEDOMETER) {
        matches++;
        if ( matches > 1) {
          // Report anonomly
          return;
        }
      }
    }
    if (matches != 1) {
      // Report anomaly
    }
}

void inspect_minimal_break(frame * frames, size_t count) {
  if (count != 2) {
    // Report anomaly
  }
}
iterate_in_time_frame(input, 50, inspect_speedometer_frequency)
iterate_in_time_frame(input, 5, inspect_pedals_frequency)



// Setup detector
StreamAnomalyDetector detector(some_socket);
detector->RegisterTimeSliceMatcher(50, inspect_speedometer_frequency);
detector->RegisterTimeSliceMatcher(5, inspect_pedals_frequency);
detector->RegisterTimeSliceMatcher(10, inspect_minimal_break, FRAME_BREAK);
