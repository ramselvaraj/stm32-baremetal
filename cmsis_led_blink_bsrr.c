#include <stdint.h>

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

    while(1){
        GPIOA->BSRR |= (1U << 5); //turn LED on
        delay(4000000);
        GPIOA->BSRR |= (1U << 21); //turn LED off
        delay(4000000);
    }
}
