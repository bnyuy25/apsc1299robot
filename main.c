#include <stdio.h>
#include <p18f4525.h>
#include "sumovore.h"
#include "motor_control.h"
#include "interrupts.h"


// main acts as a cyclical task sequencer
void main(void)
{
   
    initialization(); // function from sumovore.c
                      // it sets up pwm (using timer2),
                      // IO pins, the ADC, the 
                      // USART and the default
                      // threshold
    printf("\n\rKwantlen APSC1299 simple curve follower -- with error codes\n\r"); 
    ClrWdt();         // defined in <p18f4525.h>

    threshold = 800u; // to change from default value
                     // uncomment and change to any unsigned int <1024u -- most usually <512u

    while(0)//robot to spin around one wheel
    {
        set_motor_speed(left, medium, 0);
        set_motor_speed(right, stop, 0);
    }
    while(0)//robot to spin around in one spot
    {
        set_motor_speed(left, medium, 0);
        set_motor_speed(right, rev_medium, 0);
    }
    


    while(1)
    {
        set_motor_speed(left, medium, 0);
        set_motor_speed(right, slow, 0);
        while(0)
        {
        for(int i =0; i != 1000; i++)
            _delay(100000);
        set_motor_speed(left, medium, 0);
        set_motor_speed(right, medium, 0);
        for(int i =0; i != 1000; i++)
            _delay(100000);
        set_motor_speed(left, slow, 0);
        set_motor_speed(right, slow, 0);
        for(int i =0; i != 1000; i++)
            _delay(100000);
        set_motor_speed(left, stop, 0);
        set_motor_speed(right, stop, 0);
        for(int i =0; i != 1000; i++)
            _delay(100000);
        set_motor_speed(left, rev_slow, 0);
        set_motor_speed(right, rev_slow, 0);
        for(int i =0; i != 1000; i++)
            _delay(100000);
        set_motor_speed(left, rev_medium, 0);
        set_motor_speed(right, rev_medium, 0);
        for(int i =0; i != 1000; i++)
            _delay(100000);
        set_motor_speed(left, rev_fast, 0);
        set_motor_speed(right, rev_fast, 0);
        }
    }
    while(1)
    {
        check_sensors();    // from sumovore.c
        set_leds();         // function from sumovore.c
	                    // each LED indicates a sensor
	                    // value. If you need to use the LED's for
	                    // a different purpose change this line
	                    // and make your own LED setting function
        motor_control();    // function from motor_control.c 
        ClrWdt();           // defined in <p18f4525.h>
        if(lvd_flag_set())  LVtrap();
    }
}
 