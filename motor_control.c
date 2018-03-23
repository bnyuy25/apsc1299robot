#include "sumovore.h"
#include "motor_control.h"
#include <xc.h>

void follow_simple_curves(void);
void spin_left(void);
void turn_left(void);
void straight_fwd(void);
void turn_right(void);
void spin_right(void);
void Left_Right_angle(void);
void Right_Right_angle(void);
void dead_end(void);
void turn_around(void);


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
                       dead_end();
                       break;
         case 0b11100u:
         case 0b11000u:
         case 0b11110u:
             Left_Right_angle();
             break;
         case 0b00111u:
         case 0b00011u:
         case 0b01111u:
             Right_Right_angle();
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


void Left_Right_angle(void)
{
    while(SeeLine.B >= 0b10000)
    {
        check_sensors();
        set_leds();
    }
    set_motor_speed(left, slow, 0);
    set_motor_speed(right,slow,0);
    //for(int i =0; i != 10; i++)
       // _delay(100000);
    check_sensors();
    set_leds();
    while((SeeLine.B!=0b00100)&&(SeeLine.B!=0b00110)&&(SeeLine.B!=0b01100))
    {
    set_motor_speed(left, rev_slow, 0);
    set_motor_speed(right,slow,0); 
    check_sensors();
    set_leds();
    }
    
    
}

void Right_Right_angle(void)
{
    
    set_motor_speed(left, slow, 0);
    set_motor_speed(right,slow,0);
    for(int i =0; i != 10; i++)
        _delay(100000);
    check_sensors();
    set_leds();
    while(SeeLine.B!=0b00100 &&SeeLine.B!=0b00110 && SeeLine.B!=0b01100)
    {
    set_motor_speed(left, slow, 0);
    set_motor_speed(right,rev_slow,0); 
    check_sensors();
    set_leds();
    }
    
    
}
void dead_end(void)
{
    set_motor_speed(right,medium ,0);
    set_motor_speed(left,medium,0);
	OpenTimer0(TIMER_INT_OFF & T0_SOURCE_INT & T0_16BIT & T0_PS_1_128);
	TMR0IF = 0;
	WriteTimer0(28036);
	while ((TMR0IF != 1)&&SeeLine.B==0b00000)
	{
		check_sensors();
        set_leds();
	}
	if (TMR0IF == 1)
	{
		turn_around();
	}
}


void turn_around(void)
{
    OpenTimer0(TIMER_INT_OFF & T0_SOURCE_INT & T0_16BIT & T0_PS_1_128);
    TMR0IF = 0;
    WriteTimer0(28036);
    while(TMR0IF==0)
    {
        set_motor_speed(right, rev_medium, 0);
        set_motor_speed(left, rev_medium, 0);
    }
    OpenTimer0(TIMER_INT_OFF & T0_SOURCE_INT & T0_16BIT & T0_PS_1_256);
    TMR0IF = 0;
    WriteTimer0(28036);
    while (TMR0IF==0)
    {
        set_motor_speed(right, slow, 0);
        set_motor_speed(left, rev_slow, 0);
    } 
}
