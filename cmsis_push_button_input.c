#include <stdint.h>

#include <stm32f4xx.h>

int main(void)
{
    //enable RCC AHB1 GPIOA clock register
    RCC->AHB1ENR |= (1U << 0);

    //enable RCC AHB1 GPIOC clock register
    RCC->AHB1ENR |= (1U << 2);

    //configure PA5 as an output register
    GPIOA->MODER &= ~(3U << (5*2));
    GPIOA->MODER |= (1U << (5*2));

    //configure PC13 as input register
    GPIOC->MODER &= ~(3U << (13*2)); //need to set 26th 27th bit to 00
    GPIOC->MODER |= (0U << (13*2)); //just writing for  formality, can be skipped cause it's already 00

    while(1){
        if(!(GPIOC->IDR & (1U << 13))){
            GPIOA->BSRR |= (1U << 5);
            //turn on  the LED
        }
        else{
            GPIOA->BSRR |= (1U << 21);
            //turn  off the LED
        }
    }
}
