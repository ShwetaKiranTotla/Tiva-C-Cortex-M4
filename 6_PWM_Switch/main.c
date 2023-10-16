#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"

//A 100kHz PWM varying duty cycle waveform on Pin PF2; controlled by Module 1 PWM Generator 3 A.
//SW2 for duty cycle increase and SW1 for decrease
//Interrupt Handlers
void GPIOFHandler(void);
void SysTickHandler(void){}

void GPIO_PORTF_setup(void);
void PWMConfig(void);
void dual_switch(void);


#define frequency 100000 //100 kHz frequency
#define time_period (16000000/frequency)
#define del_duty 5 //Change in duty cycle for each button press
volatile int duty;

#define MASK_BITS 0x11 //SW1 and SW2

void GPIO_PORTF_setup(void)
{
    SYSCTL_RCGCGPIO_R |= (1<<5);        /* enable clock to GPIOF */
    GPIO_PORTF_LOCK_R = 0x4C4F434B;     /* unlock commit register */
    GPIO_PORTF_CR_R = 0x1F;             /* make PORTF0 configurable */
    GPIO_PORTF_PUR_R = 0x11;            /* enable pull up for pin 4 and 0 */
    GPIO_PORTF_DEN_R = 0x1F;            /* set PORTF pins 4-3-2-1 as digital pins */
    GPIO_PORTF_DIR_R = 0x0E;            /* set PORTF3+PORTF2+PORTF1 pin as output (LED) pin */
    /* and PORTF4 and PORTF0 as input, SW1 is on PORTF4  and SW2 is PORTF0*/
    //GPIO PortF Interrupt Configurations
    GPIO_PORTF_IM_R &= ~MASK_BITS; // mask interrupt by clearing bits
    GPIO_PORTF_IS_R &= ~MASK_BITS; // edge sensitive interrupts
    GPIO_PORTF_IBE_R &= ~MASK_BITS; // interrupt NOT on both edges
    GPIO_PORTF_IEV_R &= ~MASK_BITS; // rising edge trigger

    /* Enable interrupt generation in GPIO */
    GPIO_PORTF_ICR_R |= MASK_BITS; // clear any prior interrupt
    GPIO_PORTF_IM_R |= MASK_BITS; // unmask interrupt by setting bits

    /* Prioritize and enable interrupts in NVIC */
    NVIC_PRI7_R &= 0xFF3FFFFF;
     // interrupt priority register 7
     // bits 21-23 for interrupt 30 (port F)
    NVIC_EN0_R |= 1 << 30; // enable interrupt 30 (port F)

}

void PWMConfig(void)
{
    SYSCTL_RCGCPWM_R |= (1<<1);     // Enable PWM1 clock
    GPIO_PORTF_AFSEL_R |= (1<<2);   //Enable alternate function
    GPIO_PORTF_PCTL_R |= 0x500;     //Make PF2 as PWM output

    // Configure the PWM generator
    PWM1_3_CTL_R |= 0x00; // Disable PWM3 while configuring and select down count mode
    PWM1_3_GENA_R = 0x8C; // Set PWM3A to produce a symmetric down-counting PWM signal
    PWM1_3_LOAD_R = time_period;
    PWM1_3_CMPA_R = (duty/100)*time_period - 1;
    PWM1_3_CTL_R |= 0x01; //Enable generator 3 counter
    PWM1_ENABLE_R |= 0x040; // Enable PWM1 channel 6 Output
}

void main(void)

{
    GPIO_PORTF_setup();
    PWMConfig();
    duty=50; //Initial duty cycle is 50%
    PWM1_3_CMPA_R = (time_period * duty) / 100; //50% duty cycle to PWM upon start up
    while(1){
        //do nothing
    }
}

void GPIOFHandler(void)
{
    dual_switch();

    int wait;
    for(wait = 0; wait <1600*1000/4; wait++){}           //Debounce Delay of 0.25seconds

    GPIO_PORTF_ICR_R = MASK_BITS;
    GPIO_PORTF_IM_R |= MASK_BITS;
}

void dual_switch(void)
{
    GPIO_PORTF_IM_R &= ~MASK_BITS;

        if(GPIO_PORTF_RIS_R & 0x10)    //SW 2
        {
            if (duty < 90)
                   {
                       duty = duty + del_duty;
                   }
            if (duty >= 90){
                duty = 90;
            }
        }
        if (GPIO_PORTF_RIS_R & 0x01)    //SW 1
        {
            if (duty > 5)
                   {
                       duty = duty - del_duty;
                   }
            if (duty <= 5){
                duty = 5;
            }
        }
        if (GPIO_PORTF_RIS_R & 0x11)    //Both switches pressed at a time
        {
            duty = duty;
        }
        PWM1_3_CMPA_R = (time_period * duty) / 100;
}
