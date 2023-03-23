#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "utils.h"

// ============================================================================
void vUSART2_Init(void) {
    USART_ClockInitTypeDef USART_ClockInitStruct;
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    // Enable needed clocks for uart.
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    // Make sure you use 'GPIO_PinSource2' and NOT 'GPIO_Pin_2'.  Using the
    // latter will not work!
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

    // Setup Tx / Rx pins.
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;			// Tx Pin
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;			// Rx Pin
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Make sure syncro clock is turned off.
    USART_ClockStructInit(&USART_ClockInitStruct);
    USART_ClockInit(USART2, &USART_ClockInitStruct);

    // Use defaults (except baud rate).
    USART_StructInit(&USART_InitStructure);
    USART_InitStructure.USART_BaudRate = 38400;
    USART_Init(USART2, &USART_InitStructure);
    USART_Cmd(USART2, ENABLE);
}

// ============================================================================
void vUSART2_RX_IRQ_Init(void) {
    // Setup receive irq.
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    // Enable USART2 interrupt
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    NVIC_SetPriority(USART2_IRQn, 11);
}
