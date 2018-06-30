/*
 * Application.c
 *
 *  Created on: 30 Jun 2018
 *      Author: Gobba
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "lcd.c"
#include "adc.c"


uint8 flag = 0 ;

	ISR(INT0_vect)
	{
		LCD_displayStringRowColumn(1,0,"PRESSED");
		flag = 1;
	}
	void INT0_Init(void)
	{
		SREG  &= ~(1<<7);                   // Disable interrupts by clearing I-bit
		DDRD  &= (~(1<<PD2));               // Configure INT0/PD2 as input pin
		GICR  |= (1<<INT0);                 // Enable external interrupt pin INT0
		MCUCR |= (1<<ISC00) | (1<<ISC01);   // Trigger INT0 with the raising edge
		SREG  |= (1<<7);                    // Enable interrupts by setting I-bit
	}

void main (void)
{
	uint32 temp;
	INT0_Init(); /* initialize External Interrupt*/
	LCD_init(); /* initialize LCD */
	ADC_init(); /* intialize ADC */
	LCD_clearScreen(); /* clear LCD at the beginning */
	LCD_displayStringRowColumn(1,2,"Temp = ");

	DDRC  = DDRC | (1<<PC0);  // Configure pin PC0 in PORTC as output pin
	PORTC = PORTC | (1<<PC0); // Initialization Led is off at the beginning(Negative Logic)


	while(1)
	{
		if (flag == 1)
		{
			PORTC = PORTC ^ (1<<PC0); //toggle led state
			if(BIT_IS_SET(PORTC,0))
			{
				LCD_displayStringRowColumn(10,0,"ON");
			}
			else
			{
				LCD_displayStringRowColumn(10,0,"OFF");
			}
			flag = 0 ;
		}
		else
		{
			/*this part should be empty*/
		}

	LCD_goToRowColumn(1,10); /* display the number every time at this position */

	temp = ADC_readChannel(2); /* read channel two where the temp sensor is connect */
	_delay_ms(3000);
	LCD_intgerToString(temp);
	_delay_ms(3000);





	}

}

