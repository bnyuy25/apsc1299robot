#include "sumovore.h"
#include "motor_control.h"
#include <xc.h>

void follow_simple_curves(void);
void spin_left(void);
void turn_left(void);
void straight_fwd(void);
void turn_right(void);
void spin_right(void);
void left_ninety(void);
void right_ninety(void);

void motor_control(void)
{
     // very simple motor control
     switch(SeeLine.B)
     {
        case 0b00100u:
        case 0b00010u:
        case 0b01000u:
        case 0b00001u:
        case 0b10000u:
                       //no breaks all above readings end up here
                       follow_simple_curves();
                       break;
        case 0b00000u:
                motors_brake_all();
                break;
         case 0b111100:
         case 0b11100u:
             left_ninety();
             break;
         case 0b00111u:
             right_ninety();
             break;
             
                        
             
        default:       break;
      } 
}

void follow_simple_curves(void)
{
     if ( SeeLine.b.Center ) straight_fwd();
     else if (SeeLine.b.Left) spin_left();
     else if (SeeLine.b.CntLeft) turn_left();
     else if (SeeLine.b.CntRight) turn_right();
    else if (SeeLine.b.Right) spin_right();
}

void spin_left(void)
{
  set_motor_speed(left, rev_fast, 0); 
  set_motor_speed(right, fast, 0); 
}

void turn_left(void)
{
  set_motor_speed(left, stop, 0); 
  set_motor_speed(right, fast, 0); 
}
void straight_fwd(void)
{
  set_motor_speed(left, fast, 0); 
  set_motor_speed(right, fast, 0); 
}
void spin_right(void)
{
  set_motor_speed(left, fast, 0); 
  set_motor_speed(right, rev_fast, 0); 
}
void turn_right(void)
{
  set_motor_speed(left, fast, 0); 
  set_motor_speed(right, stop, 0); 
}
void left_ninety(void)
{
    
    while (SeeLine.B==0b11100u||SeeLine.B==0b01100u||SeeLine.B==0b11110u) 
    {
        check_sensors();
        set_leds();
    }
    _delay(170000);
    _delay(170000);
    _delay(170000);
    if (SeeLine.B==0b00000u)
    {
    while (SeeLine.B!=0b00100||SeeLine.B!=0b01100) 
     {
        set_motor_speed(left, rev_medium,0);
        set_motor_speed(right,medium, 0);
        check_sensors();
        set_leds();
     }
    }
}
void right_ninety(void)
{
   while (SeeLine.B==0b00111u||SeeLine.B==0b00110u||SeeLine.B==0b01111u) 
    {
        check_sensors();
    }
    check_sensors();
    if (SeeLine.B==0b00000u)
    {
      set_motor_speed(left, medium,0);
      set_motor_speed(right,rev_medium, 0);
    while (SeeLine.B!=0b00100) 
     {
        check_sensors();
     }
    }
}