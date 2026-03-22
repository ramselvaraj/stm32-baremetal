#include <stdint.h>

#include <stm32f4xx.h>

/*
 * steps taken outside of the editor
 * 1) check the manuals and made sure USART2 is the one that works with Virtual COM USB
 * 2) Identified which GPIOA register and the respective alternate mode enables USART2_TX by referring to Datasheet
 * 3) Identified the APB1 bus's clock rate by referring to block diagram in Datasheet
 * 4) Identified what number of bits are writable in USART's BAUD register, 16 in this case
 * */
//USART DEFINES
#define USART2_CLK_EN (1U << 17)
#define GPIOA_CLK_EN (1U << 0) //will be used for PA5(LED) and PA2(ALTERNATE MODE SET FOR USART2)
#define GPIOA_MODER_PA2_USART_TX_EN_IDX (2*2) //access to PA2, alternate mode enable for USART2 TX
#define GPIOA_AFR_PA2_AF7_USART2_TX_IDX (2*4) //access to set alternate mode value for PA2
#define BAUD_RATE 115200
#define PERIPHERALCLK 16000000
#define USART2_TE_EN (1U << 3)
#define USART2_UE_EN (1U << 13)
#define USART2_SR_TXE (1U << 7) //to check if USART2 Data register has been set, we check the Status Register SR

static uint16_t calculate_baud_rate(uint32_t PeripheralCLK, uint32_t BaudRate){
    //why uint16? because only 16 bits can be written on USART BAUD register
    return ((PeripheralCLK + (BaudRate/2))/BaudRate);
    //why this formula? because we would like 10.2 to be rounded off to 10 and 10.8 to be rounded off to 11
}

void usart_send_char(int ch){
    while (!(USART2->SR) & USART2_SR_TXE){
        //will wait till DR is clear
    }
    USART2->DR = (ch & 0xFF);
    //set the data register with the character, converted to 8 bits
}

int main(void)
{

    /*
     * CLK ENABLES
     * */
    //enable RCC AHB1 GPIOA clock register
    RCC->AHB1ENR |= GPIOA_CLK_EN;
    //enable RCC APB1 USART2 clock register
    RCC->APB1ENR |= USART2_CLK_EN;

    /*
     * CONFIGURE GPIO REGISTER MODES
     * */
    //configure PA2 to enter alternate function mode - USART2 TX enable
    GPIOA->MODER &= ~(3U << GPIOA_MODER_PA2_USART_TX_EN_IDX);
    GPIOA->MODER |= (2U << GPIOA_MODER_PA2_USART_TX_EN_IDX);

    //configure PA2's alternate low function mode to AF7 for USART2 TX
    GPIOA->AFR[0] &= ~(15U << GPIOA_AFR_PA2_AF7_USART2_TX_IDX);
    GPIOA->AFR[0] |= (7U << GPIOA_AFR_PA2_AF7_USART2_TX_IDX);

    /*
     * CONFIGURE USART2 REGISTER
     * */
    USART2->BRR = calculate_baud_rate(PERIPHERALCLK,BAUD_RATE);
    USART2->CR1 = USART2_TE_EN; //enable Transmission Enable bit
    USART2->CR1 |= USART2_UE_EN; //enable USART enable bit


    /*
     * MAIN LOOP
     * */
    while(1){
        usart_send_char('A');
    }
}
