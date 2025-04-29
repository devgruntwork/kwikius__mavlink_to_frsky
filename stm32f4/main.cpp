/**
    * Perform a startup LED sequence to indicate the system is alive.
    */
   void do_startup_leds()
   {
      turn_on_led<blink_led>();
      turn_on_led<mavlink_heartbeat_led>();

      wait(ms{1000});

      turn_off_led<mavlink_heartbeat_led>();

      wait(ms{500});
      turn_off_led<blink_led>();
   }