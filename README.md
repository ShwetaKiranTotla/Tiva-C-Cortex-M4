# ARM_CortexM4
Microcontroller Programming on TI Tiva C Series TM4C123GH6PM, ARM Cortex M4 core
## Installation Guide
This repository uses TI's Code Composer Studio to run code on the TM4C123GH6PM. The software packages are:  
[For Windows](https://drive.google.com/drive/folders/1XO4oXxN1Qro8AKj1kSJ2egRrKwd1VQq4?usp=drive_link)  
[For Linux/Ubuntu](https://drive.google.com/drive/folders/1Lose8u8mDRVsEM2teKaD3896w4Rmm2ii?usp=drive_link)  
After successful installation, create a new project (with main.c) and add the board's [header file](https://github.com/ShwetaKiranTotla/Tiva-C-Cortex-M4/blob/main/1_LED_Blink/tm4c123gh6pm.h), if not already present.
## Contents
[Getting Started](https://github.com/ShwetaKiranTotla/Tiva-C-Cortex-M4/tree/main/1_LED_Blink)  
Code for blinking an led on the Tiva board  

[GPIO Switch Control](https://github.com/ShwetaKiranTotla/Tiva-C-Cortex-M4/tree/main/2_GPIO_Switch_Control)  
Control a switch using GPIO to turn on an LED  

[Sequential LED](https://github.com/ShwetaKiranTotla/Tiva-C-Cortex-M4/tree/main/3_Sequential_LED)  
Control a switch using GPIO to turn on an LED in a sequential pattern: Red-->Green-->Blue. Each button press turns on either red, blue or green and the next button press turns on the next LED in the sequence and so on.  

[Delay using Systick](https://github.com/ShwetaKiranTotla/Tiva-C-Cortex-M4/tree/main/4_Systick_Delay)  
Generate a delay using systick timer of the ARM Cortex M4 core.  

[Systick and GPIO Interrupts](https://github.com/ShwetaKiranTotla/Tiva-C-Cortex-M4/tree/main/5_GPIO_Systick_Interrupts)  
Use Systick and GPIO interrupts to turn on an LED for a fixed duration.  

[PWM control by switches](https://github.com/ShwetaKiranTotla/Tiva-C-Cortex-M4/tree/main/6_PWM_Switch)  
Generate a PWM signal whose duty cycle can be controlled by GPIO Switches.  

[PWM control by duration](https://github.com/ShwetaKiranTotla/Tiva-C-Cortex-M4/tree/main/7_PWM_Duration)  
Generate a PWM signal whose duty cycle can be controlled by duration of the switch press.  

[UART](https://github.com/ShwetaKiranTotla/Tiva-C-Cortex-M4/tree/main/8_UART)  
UART character receive and transmit using interrupts.  

## References
1. [Systick Timer Interrupt Programming TM4C123 ARM Cortex M4](https://microcontrollerslab.com/systick-timer-interrupt-programming-tm4c123-arm-cortex-m4/)
2. [GPIO Interrupts TM4C123 Tiva Launchapd - Edge Triggered and Level](https://microcontrollerslab.com/gpio-interrupts-tm4c123-tiva-launchpad-edge-level-triggered/)
3. [Using SysTick with Interrupt | Embedded systems](https://labs.dese.iisc.ac.in/embeddedlab/using-systick-with-interrupt/)
4. [LuisAfonso95/TM4C123-Launchpad-Examples](https://github.com/LuisAfonso95/TM4C123-Launchpad-Examples/tree/master)
5. [LPC1768: SysTick Timer - Tutorials](https://www.exploreembedded.com/wiki/LPC1768:_SysTick_Timer)
6. [Starter files for embedded systems](https://users.ece.utexas.edu/~valvano/arm/)
7. https://users.ece.utexas.edu/~valvano/arm/tm4c123gh6pm.s  

