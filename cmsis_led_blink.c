#include <stdint.h>

//included stm32f4xx.h, which contains the cmsis of this board
/*
 * additional steps taken in project settingsof STM32CUBE IDE
 * 	1) import the Drivers folder (containing the Device and Includes) from the board's tools section on the STM32 website
 * 	2) Add the Device/Include and Drivers/Include folders to the project C/C++ includes
 * 	3) visit the stm32f4xx.h file and find the specific boards 'defined' name, and add it as a SYMBOL in project settings
 * */

#include <stm32f4xx.h>

static void delay(volatile uint32_t count)
{
    while (count--) //count will be a CPU clock
    {
        __asm volatile ("nop"); //inject raw assembly, nop instruction
    }
}

int main(void)
{

    //enable RCC AHB1 GPIOA clock register
    RCC->AHB1ENR |= (1U << 0);

    //configure PA5 as an output register
    GPIOA->MODER &= ~(3U << (5*2));
    GPIOA->MODER |= (1U << (5*2));

    for (int i = 0; i < 311; ++i)
    {
        // Turn LED ON (set bit 5)
        GPIOA->ODR |=  (1U << 5);
        delay(4000000);   // adjusted for 16 MHz clock of F446RE

        // Turn LED OFF (clear bit 5)
        GPIOA->ODR &= ~(1U << 5);
        delay(4000000);   // adjusted for 16 MHz clock of F446RE
    }


    /* Loop forever */
    for(;;);
}
