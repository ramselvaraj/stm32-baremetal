#include <stdint.h>

#define RCC_BASE 0x40023800UL
#define GPIOA_BASE  0x40020000UL

#define RCC_AHB1ENR *(volatile uint32_t*)(RCC_BASE + 0x30) //RCC clock enable register for AHB1 Bus
#define GPIOx_MODER *(volatile uint32_t*)(GPIOA_BASE + 0x00) //gpioA mode register
#define GPIOx_ODR *(volatile uint32_t*)(GPIOA_BASE + 0x14) //gpioA output data register

int main(void)
{

    //enable RCC AHB1 GPIOA clock register
    RCC_AHB1ENR |= (1U << 0); //basically a 32 bit mask, so only the first bit gets enabled to 1 no matter what it's original value was

    GPIOx_MODER &= ~(3U << (5*2));
    GPIOx_MODER |= (1U << (5*2));
    /*
     * we are trying to set the 10th and 11th bit (corresponding to PA5) to 01 (1U), while retaining the previous bits as is
     * so we first generate a shifted bit mask of 11 by 10 positions (so 10 and 11) (2*5 cause each PAx uses 2 bits for representation)
     * then we take the inverse of it to generate a number who's 10th and 11th bits are for sure 00, while the rest are 1's
     * we then AND it with the original to make sure those two positions now are for sure 00, while the rest remain unchanged
     * we can now safely use the 32bit mask of 01 shifted 10 positions to set the bits we actually want with the OR operation.
     * */

    //write the 5th position of ODR of GPIOA
    GPIOx_ODR |= (1U << 5);


    /* Loop forever */
    for(;;);
}
