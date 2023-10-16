#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"

//GPIO PORTF LEDs
#define RED_LED (1 << 1)
#define BLUE_LED (1 << 2)
#define GREEN_LED (1 << 3)

/* SysTick memory-mapped registers */
#define STCTRL *((volatile long *) 0xE000E010)    // control and status
#define STRELOAD *((volatile long *) 0xE000E014)    // reload value
#define STCURRENT *((volatile long *) 0xE000E018)    // current value
#define COUNT_FLAG  (1 << 16)   // bit 16 of CSR automatically set to 1
                                //   when timer expires
#define ENABLE      (1 << 0)    // bit 0 of CSR to enable the timer
#define TICKINT     (1 << 1)    // bit 1 of CSR to enable the interrupt
#define CLKINT      (1 << 2)    // bit 2 of CSR to specify CPU clock

#define CLOCK_MHZ 16

#define MASK_BITS 0x11 // pins for which interrupts should be enabled

void GPIO_PORTF_setup(void){
    SYSCTL_RCGC2_R |= 0x00000020;      /* enable clock to GPIOF */
    GPIO_PORTF_LOCK_R = 0x4C4F434B;     /* unlock commit register */
    GPIO_PORTF_CR_R = 0x01;             /* make PORTF0 configurable */
    GPIO_PORTF_DIR_R = 0x0E;            /* set PORTF3+PORTF2+PORTF1 pin as output (LED) pin */
                                        /* and PORTF4 and PORTF0 as input, SW1 is on PORTF4  and SW2 is PORTF0*/
    GPIO_PORTF_DEN_R = 0x1F;            /* set PORTF pins 4-3-2-1 as digital pins */
    GPIO_PORTF_PUR_R = 0x11;            /* enable pull up for pin 4 and 0 */
    GPIO_PORTF_LOCK_R = 0;     /* lock commit register */
}

void GPIO_PORTF_Interrupt(void){
    GPIO_PORTF_IM_R &= ~MASK_BITS; // mask interrupt by clearing bits
    GPIO_PORTF_IS_R &= ~MASK_BITS; // edge sensitive interrupts
    GPIO_PORTF_IBE_R &= ~MASK_BITS; // interrupt NOT on both edges
    GPIO_PORTF_IEV_R &= ~MASK_BITS; // rising edge trigger
    /* Prioritize and enable interrupts in NVIC */
    NVIC_PRI7_R = (NVIC_PRI7_R & 0xF1FFFFFF) | (3 << 21);
     // interrupt priority register 7
     // bits 21-23 for interrupt 30 (port F)
    NVIC_EN0_R |= 1 << 30; // enable interrupt 30 (port F)
    /* Enable interrupt generation in GPIO */
    GPIO_PORTF_ICR_R = MASK_BITS; // clear any prior interrupt
    GPIO_PORTF_IM_R |= MASK_BITS; // unmask interrupt by setting bits
}

int main(void)
{
    GPIO_PORTF_setup();
    GPIO_PORTF_Interrupt();

    while (1){

    }

}

void SysTickHandler(void)
{
    GPIO_PORTF_DATA_R = 0x0; //Turn off LED when after the systick interrupt is generated (after 1s)
    GPIO_PORTF_IM_R |= MASK_BITS;; // Unmask interrupt by setting bits
}

void GPIOFHandler(void){
    if (GPIO_PORTF_MIS_R & 0x10){ //If SW1 is Pressed
        GPIO_PORTF_DATA_R |= RED_LED;
        STRELOAD = 16000000 -1; // Reload value for 1s
        STCTRL |= (CLKINT) | (ENABLE)|(TICKINT); //Enable processor clock, timer and interrupt
        GPIO_PORTF_IM_R &= ~MASK_BITS;
        GPIO_PORTF_ICR_R |= 0x10; //Clear Interrupt Flag
    }
    if (GPIO_PORTF_MIS_R & 0x01){ //If SW2 is Pressed
        GPIO_PORTF_DATA_R |= BLUE_LED;
        STRELOAD = 16000000 -1; // Reload value for 1s
        STCTRL |= (CLKINT) | (ENABLE)|(TICKINT); //Enable processor clock, timer and interrupt
        GPIO_PORTF_IM_R &= ~MASK_BITS;
        GPIO_PORTF_ICR_R |= 0x01; //Clear Interrupt Flag
    }

}
