#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"

/**
 * main.c
 */

//GPIO PORTF LEDs
#define RED_LED (1 << 1)
#define BLUE_LED (1 << 2)
#define GREEN_LED (1 << 3)

void GPIO_PORTF_setup(void){
    SYSCTL_RCGC2_R |= 0x00000020;      /* enable clock to GPIOF */
    GPIO_PORTF_LOCK_R = 0x4C4F434B;     /* unlock commit register */
    GPIO_PORTF_CR_R = 0x01;             /* make PORTF0 configurable */
    GPIO_PORTF_DIR_R = 0x0E;            /* set PORTF3+PORTF2+PORTF1 pin as output (LED) pin */
                                        /* and PORTF4 and PORTF0 as input, SW1 is on PORTF4  and SW2 is PORTF0*/
    GPIO_PORTF_DEN_R = 0x1F;            /* set PORTF pins 4-3-2-1 as digital pins */
    GPIO_PORTF_PUR_R = 0x11;            /* enable pull up for pin 4 and 0 */
    //GPIO_PORTF_LOCK_R = 0;     /* lock commit register */
}

int main(void)
{
    GPIO_PORTF_setup();
    int count = 0;
    while (1){
       if (count==0){
           GPIO_PORTF_DATA_R |= RED_LED; //Light up Red LED on Startup
       }
        if ((GPIO_PORTF_DATA_R & 0x01)==0)
        {
                count = count +1;//Increase count for each button press
            }

        if (count % 3 == 0){
                    GPIO_PORTF_DATA_R &= ~GREEN_LED;
                    GPIO_PORTF_DATA_R |= RED_LED;
                    while((GPIO_PORTF_DATA_R & 0x01)==0){} //Implementing debouncing by waiting till the switch is released

                }


            if (count % 3 == 1){
                    GPIO_PORTF_DATA_R &= ~RED_LED;
                    GPIO_PORTF_DATA_R |= BLUE_LED;
                    while((GPIO_PORTF_DATA_R & 0x01)==0){}

                }

            if (count % 3 == 2){
                    GPIO_PORTF_DATA_R &= ~BLUE_LED;
                    GPIO_PORTF_DATA_R |= GREEN_LED;
                    while((GPIO_PORTF_DATA_R & 0x01)==0){}
                }

        }
}
