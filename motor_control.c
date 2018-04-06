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
void Left_acute_angle(void);
void Right_acute_angle(void);
void Track_End(void);

int x = 14000;//timer for both acute angles, decrease this value is acute angles are super thick
void motor_control(void)
{
    
     switch(SeeLine.B)
     {
        case 0b00001u:
            _delay(100000);
            _delay(100000);
            _delay(100000);
            turn_left();
            break;
        case 0b10000u:
                        _delay(100000);
            _delay(100000);
            _delay(100000);

            turn_right();
            break;          
        case 0b00100u:
        case 0b00010u:
        case 0b01000u:
                        //no breaks all above readings end up here
                       follow_simple_curves();
                       break;
       case 0b00000u:
                       dead_end();
                       break;
         case 0b01100u:
             spin_left();
             break;
         case 0b000110u:
             spin_right();
             break;
         case 0b11100u:
         case 0b11110u:
             Left_Right_angle();
             break;
         case 0b00111u:
         case 0b01111u:
             Right_Right_angle();
             break;
         case 0b10100u:
         case 0b10110u:
             Left_acute_angle();
             break;
         case 0b01101u:
         case 0b00101u:
             Right_acute_angle();
             break;
        case 0b11111u:
            Track_End();
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
  set_motor_speed(left, rev_slow, 0); 
  set_motor_speed(right, slow, 0); 
}

void turn_left(void)
{
  set_motor_speed(left, stop, 0); 
  set_motor_speed(right, slow, 0); 
}
void straight_fwd(void)
{
  set_motor_speed(left, medium, 0); 
  set_motor_speed(right, medium, 1); 
}
void spin_right(void)
{
  set_motor_speed(left, slow, 0); 
  set_motor_speed(right, rev_slow, 0); 
}
void turn_right(void)
{
  set_motor_speed(left, slow, 0); 
  set_motor_speed(right, stop, 0); 
}
void Left_Right_angle(void)
{
    set_motor_speed(left, slow, 0);
    set_motor_speed(right,slow,0);
    check_sensors();
    set_leds();
    for(int i =0; i!=15;i++)
        _delay(100000);
    
    check_sensors();
    set_leds();
    if(SeeLine.B == 0)
    {
    while((SeeLine.B!=0b00100)&&(SeeLine.B!=0b00110))
    {
    set_motor_speed(left, rev_fast, 0);
    set_motor_speed(right, fast,0); 
    check_sensors();
    set_leds();
    }
    }    
}

void Right_Right_angle(void)
{
    set_motor_speed(left, slow, 0);
    set_motor_speed(right,slow,0);
    check_sensors();
    set_leds();
    
    for(int i =0; i!=15;i++)
        _delay(100000);
    
    check_sensors();
    set_leds();
    if(SeeLine.B == 0)
    {
    while((SeeLine.B!=0b00100)&&(SeeLine.B!=0b01100))
    {
    set_motor_speed(left, fast, 0);
    set_motor_speed(right, rev_fast,0); 
    check_sensors();
    set_leds();
    }
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

void Left_acute_angle(void)
{
    while(SeeLine.B >= 0b10000)
    {
        check_sensors();
        set_leds();               //can we like function call follow simple curves and SLB - 0b100000
        //to like make sure it follows the line in the case of close tracks
    }
    set_motor_speed(right, medium, 0);
        set_motor_speed(left, medium, 0);
    OpenTimer0(TIMER_INT_OFF & T0_SOURCE_INT & T0_16BIT & T0_PS_1_64); //using prescale 64 because we want 5 cmish worth of distances
    TMR0IF = 0;
    WriteTimer0(x); //((2^16-28036) *64* 125*10^-9 = 0.35 // 21786
    while(TMR0IF==0 )
    {
        check_sensors();
        set_leds();   
    }
    if (SeeLine.B==0b01100||SeeLine.B==0b00100||SeeLine.B==0b00110||SeeLine.B==0b01110)
        return;
   //need to have more triggers for returning?? UNSURE PLZ CONFIRM
    
    
    while(SeeLine.B!=0b01100&&SeeLine.B!=0b00100&&SeeLine.B!=0b00110&&SeeLine.B!=0b01110) //in the case of close tracks its gonna be 0b00101 for a longer period of time
        {
            check_sensors();
            set_leds();   
            set_motor_speed(right,medium,0);
            set_motor_speed(left,rev_medium,0);
        }
            
    
    
}

void Right_acute_angle(void)//the problem is that during 2 close tracks, it will go into the code, then stay in the first loop and just go straight from follow simple curves
{
    while(SeeLine.B == 0b01111||SeeLine.B==0b00111||SeeLine.B==0b00101||SeeLine.B==0b00011||SeeLine.B==0b01101)//while(SLB %2 ==1)?
    {
        check_sensors();
        set_leds(); //need a SLB-1 follow simple curves 
    }
    set_motor_speed(right, medium, 0);
    set_motor_speed(left, medium, 0);
    OpenTimer0(TIMER_INT_OFF & T0_SOURCE_INT & T0_16BIT & T0_PS_1_64); //using prescale 64 because we want 5 cmish worth of distances
    TMR0IF = 0;
    WriteTimer0(x); //((2^16-28036) *64* 125*10^-9 = 2^16- 0.5/64/125/10^-9
    while(TMR0IF==0)
    {
        check_sensors();
        set_leds();   
    }
    if (SeeLine.B==0b01100||SeeLine.B==0b00100||SeeLine.B==0b00110||SeeLine.B==0b01110)
        return;
    //may need more triggers????
    
    while(SeeLine.B!= 0b00100 && SeeLine.B!= 0b01110 &&SeeLine.B!= 0b00110)
        {
            check_sensors();
            set_leds();   
            set_motor_speed(right,rev_medium,0);
            set_motor_speed(left,medium,0);
        }
    //while loop until SLB is not 01111 || 0b00111|| 0b00101 || 0b00011 || 0b01101
    //motor speed medium
    //set a timer for 5cm? ish of distances, delat until finished
    //if middle-ish on line exit
    //while middle bit not on line turn in place
    
    
}          
  void Track_End(void)
{
  set_motor_speed(left, slow, 0);
    set_motor_speed(right,slow,0);
    check_sensors();
    set_leds();
   
    for(int i =0; i!=25;i++) //needs to be bigger
        _delay(100000);
    
    
    check_sensors();
    set_leds();
    if (SeeLine.B==0b11111)
    {
     set_motor_speed(left, stop, 0);
    set_motor_speed(right,stop,0);
    while(SeeLine.B==0b11111)
        check_sensors();
    }
    
    
}
  
//  may need to return on track end delay
  
  
//Left right angle backup code
//set_motor_speed(left, slow, 0);
   // set_motor_speed(right,slow,0);
    //while(SeeLine.B!=0b00000&&SeeLine.B!=0b00100&&SeeLine.B!=0b00110&&SeeLine.B!=0b01100)
    //{
   //     check_sensors();
//        set_leds();
  //  }
   // _delay(100000);
   // set_motor_speed(left, slow, 0);
   // set_motor_speed(right,slow,0);
   // check_sensors();
    //set_leds();
    //while((SeeLine.B!=0b00100)&&(SeeLine.B!=0b00110)&&(SeeLine.B!=0b01100))
    //{
    //set_motor_speed(left, rev_slow, 0);
    //set_motor_speed(right,slow,0); 
    //check_sensors();
    //set_leds();
   // }
  
  //may need to change threshold