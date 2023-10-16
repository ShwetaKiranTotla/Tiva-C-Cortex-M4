# ARM_CortexM4
Microcontroller Programming on TI Tiva C Series TM4C123GH6PM, ARM Cortex M4 core

## Contents
[Getting Started](link here)
Code for blinking an led on the Tiva board
<br>
[GPIO Switch Control](link here)
Control a switch using GPIO to turn on an LED
<br>
[Sequential LED](link here)
Control a switch using GPIO to turn on an LED in a sequential pattern: Red-->Green-->Blue. Each button press turns on either red, blue or green and the next button press turns on the next LED in the sequence and so on.
<br>
[Delay using Systick](link here)
Generate a delay using systick timer of the ARM Cortex M4 core.
<br>
[Systick and GPIO Interrupts](link here)
Use Systick and GPIO interrupts to turn on an LED for a fixed duration.
<br>
[PWM control by switches]
Generate a PWM signal whose duty cycle can be controlled by GPIO Switches.
<br>
[PWM control by duration] 
Generate a PWM signal whose duty cycle can be controlled by duration of the switch press.
<br>
[UART]
UART character receive and transmit using interrupts.

## References
1. [Systick Timer Interrupt Programming TM4C123 ARM Cortex M4](https://microcontrollerslab.com/systick-timer-interrupt-programming-tm4c123-arm-cortex-m4/)
2. [GPIO Interrupts TM4C123 Tiva Launchapd - Edge Triggered and Level](https://microcontrollerslab.com/gpio-interrupts-tm4c123-tiva-launchpad-edge-level-triggered/)
3. [Using SysTick with Interrupt | Embedded systems](https://labs.dese.iisc.ac.in/embeddedlab/using-systick-with-interrupt/)
4. [LuisAfonso95/TM4C123-Launchpad-Examples](https://github.com/LuisAfonso95/TM4C123-Launchpad-Examples/tree/master)
5. [LPC1768: SysTick Timer - Tutorials](https://www.exploreembedded.com/wiki/LPC1768:_SysTick_Timer)
6. [Starter files for embedded systems](https://users.ece.utexas.edu/~valvano/arm/)
7. https://users.ece.utexas.edu/~valvano/arm/tm4c123gh6pm.s  

