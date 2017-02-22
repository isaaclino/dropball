
/* 
 *  ----  DROP BALL ---
 
 * Isaac Lino  ilino001@ucr.edu
 
 * Lab Section: 24
 * Assignment: Final Project
 *
  1 Use the LCD code, along with a button and/or time delay to display the message 
    "Insert coin". 
	
  2 The LCD will ask to drop a ball and the game it will start after the coin it is inserted 
  and 3 7-Seg display will spin Until a button/sensor stop it	
  
  3 Send the value of the 7-Seg to LCD to display a score
	
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */

#define F_CPU	4000000UL

#include "bit.h"
#include "io.h"
#include "io.c"
#include "timer.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <compat/deprecated.h>


// --------------Task scheduler data structure----------------- //
typedef struct _task
{
	//Task's current state, period, and the time elapsed
	// since the last tick
	signed char state;
	unsigned long int period;
	
	//Task tick function
	unsigned long int elapsedTime;
	int (*TickFct)(int);
} task;
// ----------------- End Task scheduler --------------------- //

// ------------------ Shared variables ---------------------- //
unsigned char SM1_output = 0x00;
volatile unsigned char zero=0x40, one=0xF9, two=0xA4, three=0x30, four=0x99, five=0x92, six=0x02, seven=0xF8, eight=0x00, nine=0x90; 
volatile unsigned char first_seg=0, second_seg=0, third_seg=0;
unsigned char seg[10]={0x40, 0xF9, 0xA4, 0x30, 0x99, 0x92, 0x02, 0xF8, 0x00, 0x90};
unsigned char randomseg[3]={0xFE, 0x3F, 0xF7};	
unsigned char score = 0;	
unsigned char lives = 3;

// ---------------- End Shared variables -------------------- //

// ------------------ User-Defined FSMs --------------------- //
enum SM1_States {SM1_start, SM1_instertcoin, SM1_wait_coin, SM1_7seg, SM1_wait_balldrop, SM1_hole1, SM1_hole2, SM1_hole3, SM1_playagain, SM1_finalscore};
// ---------------- End User-Defined FSMs ------------------- //

// ----------- Function from User Defined FSMs -------------- //
int SMTick1(int state)
{
	char A0 = PINA & 0x01; //  A0 button or Photo-sensor to start the game "insert the coin"
    char A2 = ~PINA & 0x04; // button or sensor to stop 1st 7-seg 
    char A3 = ~PINA & 0x08; // button or sensor to stop 2st 7-seg 
	char A4 = ~PINA & 0x10; // button or sensor to stop 3st 7-seg 
	//char A0 = ~PINA & 0x80; // button or sensor to start the game "insert the coin"
	DDRC = 0xFF;	// Configure port C as output
	
    switch (state) // ------ STATE TRANSITIONS --------
    {
		
        case SM1_start:
            state = SM1_instertcoin;
            break;

        case SM1_instertcoin:
		if (!A0)
		{
			state = SM1_wait_coin;	
	    }		
		else
		{
			state = SM1_instertcoin;
		}	
        break;
		
		
		
		case SM1_wait_coin:
		if (!A0)
		{	
			state = SM1_wait_coin;
		}
		else
		{
			state = SM1_7seg;			
		}			
	    break;


		case SM1_7seg:
		if (!A2)
		{
			state = SM1_wait_balldrop;
		}		
		else
		{
			state = SM1_7seg;
		}			
		break;
		
		
		case SM1_wait_balldrop:
			if(A2)
				{
					state = SM1_hole1;
				}	
			else if(A3)
				{
					state = SM1_hole2;	
				}
			else if(A4)
				{
					state = SM1_hole3;
				}
			else
				{
					state = SM1_wait_balldrop;
				}		
		break;	
		
		case SM1_hole1:
			if (A2)
			{
				state = SM1_hole1;
			}
			else
			{
				state =SM1_playagain;
			}		
		break;
		
		
		case SM1_hole2:
			if (A3)
			{
				state = SM1_hole2;
			}
			else
			{
				state =SM1_playagain;
			}		
		break;
		
		
		case SM1_hole3:
			if (A4)
			{
				state = SM1_hole3;
			}
			else
			{
				state =SM1_playagain;
			}		
		break;
		
		
		case SM1_playagain:
		
		
		if (lives == 0)
		{
			state = SM1_finalscore;
		} 
		
		else
		{
			state = SM1_playagain;
		}
		break;	
		
		
		case SM1_finalscore:
		if (!A0)
		{
			state = SM1_finalscore;
		}
		else
		{
			
			state = SM1_start;
		}	
		break;	
			
			
        default: state = SM1_start; 
		break;
    }   
		
		
   
   
    switch (state) // ---- STATE ACTIONS -------
    {
		
		case SM1_start: 
		score = 0;
		lives = 3;
		
		break;   
     
	    
		case SM1_instertcoin:
		if (score == 0)
		{
		LCD_init();                               //Initialize LCD		
		LCD_DisplayString(1,"~ Insert a Coin!");  //Display message wait until insert a coin
		} 
		else
		{			
			LCD_ClearScreen();
			LCD_DisplayString(1, " Final Score:  ");
			LCD_WriteData(score + '0');
			_delay_ms(3000);
			
			//LCD_ClearScreen();
			//LCD_DisplayString(1, "  !Game  Over!   Insert a Coin! ");
			//_delay_ms(3000);
			
			LCD_ClearScreen();
			LCD_DisplayString(1, "~ Insert a Coin!  Last Score:  ");
			LCD_WriteData(score + '0');
			_delay_ms(3000);					
		}
        break;
 
 
		case SM1_wait_coin:
		 PORTC = 0x3F; // Waiting for coin - 7seg in stand by
		 PORTB = 0x3F; // Waiting for coin - 7seg in stand by
		break;


		case SM1_7seg: 
		 LCD_ClearScreen();
		 LCD_DisplayString(1, "Drop the ball...");
		 score = 0;
		 lives = 3;
		break;
		 
		 
		case SM1_wait_balldrop:
		 //  PORTB = 0x01; // Activates 1rd 7seg
		 //  PORTB = 0x02; // Activates 2rd 7seg
		 //  PORTB = 0x04; // Activates 3rd 7seg
		PORTB = 0x07; // Activates ALL 7seg		
		PORTC = zero;
		break;
		
		
		case SM1_hole1:
		  PORTB = 0x01;  // Activates 1rd 7seg
		  PORTC = one;   // Writes 1 into first 7seg
		  LCD_ClearScreen();
		  LCD_DisplayString(1, "You Got 1 Point ");
		  _delay_ms(3000);
		  score += 1;
		  lives -=1;
		  state = SM1_playagain;
		break;
		
		
		case SM1_hole2:
		  PORTB = 0x02;  // Activates 2rd 7seg
		  PORTC = two;   // Writes 2 into second 7seg
		  LCD_ClearScreen();
		  LCD_DisplayString(1, "You Got 2 Points");
		  _delay_ms(3000);
		  score += 2;
		  lives -=1;
		  state = SM1_playagain;
		break;
		
		
		case SM1_hole3:
		  PORTB = 0x04;   // Activates 3rd 7seg
		  PORTC = three;  // Writes 3 into third 7seg
		  LCD_ClearScreen();
		  LCD_DisplayString(1, "You Got 3 Points");
		  _delay_ms(3000);
		  score += 3;
		  lives -=1;
		state = SM1_playagain;
		break;
	

		case SM1_playagain:
			LCD_ClearScreen();
			LCD_DisplayString(1,"  Lives left:  ");
			LCD_WriteData(lives + '0');
			_delay_ms(4500);
			
			LCD_ClearScreen();
			LCD_DisplayString(1, "Drop ball again Your Score is: ");
			LCD_WriteData(score + '0');
		state = SM1_wait_balldrop;
		break;
			
		case SM1_finalscore: 
		PORTC = 0x3F; // Waiting for coin - 7seg in stand by
		PORTB = 0x3F; // Waiting for coin - 7seg in stand by
		
		LCD_ClearScreen();
		LCD_DisplayString(1, " Final Score:  ");
		LCD_WriteData(score + '0');
		_delay_ms(4000);	 
		 
		LCD_ClearScreen();
		LCD_DisplayString(1, "  !Game  Over!   Insert a Coin! ");	
		_delay_ms(4000);
       
	    state = SM1_start;
	//	 state = SM1_finalscore;
		break;

		
		default: 
	//	 state = SM1_start;
		break;
    }
    return state;
}
// --------- End Function from User Defined FSMs ------------ //


// --- Implement scheduler code from PES to Main Function ---//
int main(void)
{
	// Set Data Direction Registers
//	DDRA = 0x03; PORTA = 0xFC;
	
	DDRA = 0xC0; PORTA = 0x3F;
	DDRD = 0xFF; PORTD = 0x00;
	//DDRB = 0xFF; PORTB = 0x00;
	 
    // Period for tasks
	unsigned long int SMTick1_calc = 10;
	unsigned long int SMTick2_calc = 1;
    //unsigned long int SMTick3_calc = 1000;
	//unsigned long int SMTick4_calc = 10;
     
    // Calculating GCD
    unsigned long int tmpGCD = 10;
	tmpGCD = findGCD(SMTick1_calc, SMTick2_calc);
	//tmpGCD = findGCD(tmpGCD, SMTick3_calc);
	//tmpGCD = findGCD(tmpGCD, SMTick4_calc);
	
	//Greatest common divisor for all tasks
	// or smallest time unit for tasks.
	unsigned long int GCD = tmpGCD;
     
    // Recalculate GCD for Scheduler
    unsigned long int SMTick1_period = SMTick1_calc/GCD;
	//unsigned long int KP_period = SMTick1_calc/GCD;
     
    // Declare an array of tasks
    static task task1;
    task *tasks[] = {&task1};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
     
    // SM Task 1
    task1.state = -1;
    task1.period = SMTick1_period;
    task1.elapsedTime = SMTick1_period;
    task1.TickFct = &SMTick1;
     
	//Set timer and turn it on 
    TimerSet(GCD);
    TimerOn();
     
	//Scheduler for-loop iterator 
    unsigned short i;
    while (1)
    {			
		//Schedule code
        for (i = 0; i < numTasks; i++)
        {
			//Task is ready to tick
            if (tasks[i]->elapsedTime  == tasks[i]->period)
            {
				//Set next state for task
                tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				//Reset elapse time for next tick
                tasks[i]->elapsedTime = 0;
            }
            tasks[i]->elapsedTime += 1;
        }
        while (!TimerFlag);
        TimerFlag = 0;
		
		
		
		
		
    }
    // Program should not exit 
    return 0;
}