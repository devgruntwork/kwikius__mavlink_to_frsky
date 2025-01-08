/**
    * Update the LED states based on the MavLink heartbeat and system status.
    */
   ms mavlink_timer{0};
   ms blink_timer{0};
   uint32_t cur_num_heartbeats = 0;
   void update_leds()
   {
      // Get the current number of heartbeats
      uint32_t num_heartbeats = get_num_heartbeats();

      // Check if a new heartbeat has been received
      if ( num_heartbeats > cur_num_heartbeats ){
         cur_num_heartbeats = num_heartbeats;
         mavlink_timer = millis();
         turn_on_led<mavlink_heartbeat_led>();
      } else {
         // Turn off the MavLink heartbeat LED after a 250ms pulse
         if ( led_is_on<mavlink_heartbeat_led>() && (( millis() - mavlink_timer ) >= ms{250})){
            turn_off_led<mavlink_heartbeat_led>();
         }
      }

      // Blink the system status LED at 50% duty cycle
      if (( millis() - blink_timer ) >= ms{500}){
         blink_timer = millis();
         complement_led<blink_led>();
      }
   }