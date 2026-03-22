#include <stdint.h>

/*
 * custom CMSIS for GPIOA control
 */

#define RCC_BASE 0x40023800UL
#define GPIOA_BASE  0x40020000UL

#define RCC_AHB1ENR *(volatile uint32_t*)(RCC_BASE + 0x30) //RCC clock enable register for AHB1 Bus

typedef struct {
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFR[2];
} GPIOA_TypeDef;
//structure mirrors the reference manual definitions of GPIOA registers, with their base and offsets matching this serial order

#define GPIOA ((GPIOA_TypeDef *) GPIOA_BASE)

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
    RCC_AHB1ENR |= (1U << 0); //basically a 32 bit mask, so only the first bit gets enabled to 1 no matter what it's original value was

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
