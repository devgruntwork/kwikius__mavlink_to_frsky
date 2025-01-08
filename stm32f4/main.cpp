/**
    * Send FrSky data every 20 milliseconds.
    * Note: The complex time division in FrSky_send_message may be unnecessary.
    * Additionally, timestamp information could be used to send data periodically.
    */
   ms frsky_loop_time{0};
   void update_frsky()
   {
      // Check if the FrSky data needs to be updated
      if( ( millis() - frsky_loop_time ) >= ms{20} ){
         frsky_loop_time += ms{20};
         FrSky_send_message();
      }
   }