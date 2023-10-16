#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"

/* UART Transmission, baud rate 9600, 0xF0 is transmitted if SW1 is pressed
 0xAA is transmitted if SW2 is pressed
 if "AA" is received LED turns GREEN;
 if "F0" is received, the LED turns BLUE
 if any error is detected LED turns RED
 PE4 = Rx; PE5 = Tx, UART Module 5
 */

//Interrupt Handlers
void GPIOFHandler(void); //For reading switch values
void SysTickHandler(void){}
void UART5Handler(void); //For handling the received data

char UART5_Receiver(void);
void UART5_Transmitter(unsigned char data);

/* SysTick memory-mapped registers */
#define STCTRL *((volatile long *) 0xE000E010)    // control and status
#define STRELOAD *((volatile long *) 0xE000E014)    // reload value
#define STCURRENT *((volatile long *) 0xE000E018)    // current value

#define COUNT_FLAG  (1 << 16)   // bit 16 of CSR automatically set to 1
                                //   when timer expires
#define ENABLE      (1 << 0)    // bit 0 of CSR to enable the timer
#define CLKINT      (1 << 2)    // bit 2 of CSR to specify CPU clock

#define CLOCK_MHZ 16

void Delay(float ms)
{
    STCURRENT = 0;
    STRELOAD = 16000*ms; //1000us    // reload value for 'ms' milliseconds
    STCTRL |= (CLKINT | ENABLE);        // set internal clock, enable the timer

    while ((STCTRL & COUNT_FLAG) == 0)  // wait until flag is set
    {
        ;   // do nothing
    }
    STCTRL = 0;                // stop the timer

}

//LED and Switch Macros
#define RED_LED (1 << 1)
#define BLUE_LED (1 << 2)
#define GREEN_LED (1 << 3)
#define MASK_BITS 0x11 //SW1 and SW2

//Counts for reception, transmission and errors
int rx_aa = 0;
int rx_f0 = 0;
int tx_aa = 0;
int tx_f0 = 0;
int error_count = 0;

void UARTInit(void){
    /* UART5 initialization */
    UART5_CTL_R &= 0;         /* Disable UART5 module while configuring */
    UART5_IBRD_R = 104;      /* for 9600 baud rate, integer = 104 */
    UART5_FBRD_R = 11;       /* for 9600 baud rate, fractional = 11*/
    UART5_CC_R = 0;          /*select system clock*/
    UART5_LCRH_R = 0x62;     /* data length 8-bit, odd parity bit, no FIFO */
    UART5_CTL_R = 0x301;     /* Enable UART5 module, Rx and Tx */

    /* UART5 TX5 and RX5 use PE4 and PE5. Configure them digital and enable alternate function */
    //GPIO_PORTE_DEN_R = 0x30;      /* set PE4 and PE5 as digital */
    //GPIO_PORTE_DIR_R |= (1<<5);         /*Define PE5 as output*/
    //GPIO_PORTE_AFSEL_R = 0x30;    /* Use PE4,PE5 alternate function */
    //GPIO_PORTE_AMSEL_R = 0;    /* Turn off analog function*/
    //GPIO_PORTE_PCTL_R  = 0x00110000;     /* configure PE4 and PE5 for UART */

    // Enable  UART5 interrupt
    UART5_IM_R &= ((0<<4)|(0<<5)|(0<<8));       //Mask Tx, Rx and Parity interrupts
    UART5_ICR_R &= ((0<<4)|(0<<5)|(0<<8));      //Clear Tx, Rx and Parity interrupts
    UART5_IM_R |= (1<<4);                       //Enable Rx interrupt
    NVIC_EN1_R |= 0x20000000; /* enable IRQ61 for UART5 */
    NVIC_PRI15_R &= 0xFFFF5FFF;                 //Interrupt Priority 2 to UART5

    GPIO_PORTE_LOCK_R = 0x4C4F434B;     //Unlock commit register
    GPIO_PORTE_CR_R = 0xFF;             //Enable Commit function
    GPIO_PORTE_DEN_R = 0xFF;            //Enable all pins on port E
    GPIO_PORTE_DIR_R |= (1<<5);         //Define PE5 as output
    GPIO_PORTE_AFSEL_R |= 0x30;         //Enable Alternate function for PE4 and PE5
    GPIO_PORTE_PCTL_R |= 0x00110000;    //Selecting UART function for PE4 and PE5
}

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

    /* Enable interrupt generation in GPIOF */
    GPIO_PORTF_ICR_R |= MASK_BITS; // clear any prior interrupt
    GPIO_PORTF_IM_R |= MASK_BITS; // unmask interrupt by setting bits

    /* Prioritize and enable interrupts in NVIC */
    NVIC_PRI7_R &= 0xFF3FFFFF;
     // interrupt priority register 7
     // bits 21-23 for interrupt 30 (port F)
    NVIC_EN0_R |= 1 << 30; // enable interrupt 30 (port F)

}

int main(void)
{
    SYSCTL_RCGCUART_R |= 0x20;  /* enable clock to UART5 */
    SYSCTL_RCGCGPIO_R |= 0x10;  /* enable clock to PORTE for PE4/Rx and PE5/Tx */
    UARTInit();
    GPIO_PORTF_setup();
    while (1){
        // do nothing
    }
}

void GPIOFHandler(void){
    GPIO_PORTF_IM_R &= ~MASK_BITS;//clear interrupt

        if(GPIO_PORTF_RIS_R & 0x01)    //SW 2
        {
            UART5_DR_R = 0xAA; //transmit "aa"
            tx_aa++;

        }

        if(GPIO_PORTF_RIS_R & 0x10)    //SW 1
        {
            UART5_DR_R = 0xF0; //transmit "f0"
            tx_f0++;

        }
}

void UART5Handler(void){
    UART5_IM_R &= (0<<4);   //Mask UART Rx interrupt
    UART5_ICR_R &= ~(0x010); // Clear receive interrupt

        if(UART5_DR_R == 0xAA){//AA
                GPIO_PORTF_DATA_R |= GREEN_LED;
                rx_aa++;
            }
        if(UART5_DR_R == 0xF0){//F0
                GPIO_PORTF_DATA_R |= BLUE_LED;
                rx_f0++;
            }

        if (UART5_DR_R & 0x200){/*0xF00 for all errors*/
            GPIO_PORTF_DATA_R |= RED_LED;
            error_count++;
        }
        UART5_ECR_R &= 0xFFFFFFF0;  //Clear UART errors
        GPIO_PORTF_ICR_R = MASK_BITS;
        GPIO_PORTF_IM_R |= MASK_BITS;
        UART5_IM_R |= (1<<4);    //UnMask UART Rx interrupt

}

