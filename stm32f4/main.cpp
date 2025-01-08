
/*
 Copyright (c) 2012-2013 Andy Little

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "resources.hpp"
#include "timer.hpp"
#include "led.hpp"
#include "mavlink.hpp"
#include "frsky.hpp"

void setup();

namespace {
  typedef quan::time_<int32_t>::ms ms;

  void do_startup_leds();
  void update_frsky();
  void update_leds();

}

int main()
{
   setup();

   do_startup_leds();

   for (;;){

      read_mavlink();


      update_frsky();

      // blink leds to show we are alive
      // and show MavLink heartbeat if we have it...

      update_leds();
   }
}

namespace {

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
      if ( num_heartbeats > cur_num_heartbeats){
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

} // namespace
