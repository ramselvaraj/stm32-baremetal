#include <stdint.h>

#include <stm32f4xx.h>

#define GPIOA_CLK_EN (1U << 0)
#define GPIOC_CLK_EN (1U << 2)

#define GPIOA_MODER_PA5_IDX (5*2) //access to LD2 LED
#define GPIOC_MODER_PC13_IDX (13*2) //access to USER push button

#define GPIOA_BSRR_SET_PA5_IDX 5
#define GPIOA_BSRR_RESET_PA5_IDX 21
#define GPIOC_IDR_PC13_IDX 13

int main(void)
{
    //enable RCC AHB1 GPIOA clock register
    RCC->AHB1ENR |= GPIOA_CLK_EN;

    //enable RCC AHB1 GPIOC clock register
    RCC->AHB1ENR |= GPIOC_CLK_EN;

    //configure PA5 as an output register
    GPIOA->MODER &= ~(3U << GPIOA_MODER_PA5_IDX);
    GPIOA->MODER |= (1U << GPIOA_MODER_PA5_IDX);

    //configure PC13 as input register
    GPIOC->MODER &= ~(3U << GPIOC_MODER_PC13_IDX); //need to set 26th 27th bit to 00
    GPIOC->MODER |= (0U << GPIOC_MODER_PC13_IDX); //just writing for  formality, can be skipped cause it's already 00

    while(1){
        if(!(GPIOC->IDR & (1U << GPIOC_IDR_PC13_IDX))){
            GPIOA->BSRR |= (1U << GPIOA_BSRR_SET_PA5_IDX);
            //turn on  the LED
        }
        else{
            GPIOA->BSRR |= (1U << GPIOA_BSRR_RESET_PA5_IDX);
            //turn  off the LED
        }
    }
}
