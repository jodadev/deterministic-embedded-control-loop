/**
 * @file logger.c
 * @brief USART-based logging implementation.
 *
 * Implements a lightweight formatted print function and UART write
 * routines for transmitting debug output to a host machine.
 *
 * Written by Edwin J Martinez
 */

#include <stdbool.h>
#include "logger.h"
#include "hardware.h"
#include "global.h"

//* File Functions
static char convert_to_character(int num)
{
    return  (char)(48 + num);
}

static bool is_number(char ch)
{
    return ((int)ch) >= 48 && ((int)ch) < 58;
}

static void uart_write(int ch)
{
    // Wait until the Transmit Data Register is empty (TXE flag set)
    // TXE (bit 7 in SR) indicates that DR is ready to accept new data
    // If this is not checked, writing to DR could overwrite unsent data
    static uint32_t sr_txe = 7U;
    while (!(USART2_REGS->SR & BIT(sr_txe))){}

    // Write the lower 8 bits of the data to the Data Register (DR)
    // This loads the data into the transmit buffer, which then gets
    // shifted out serially over the TX line by the hardware
    USART2_REGS->DR = (ch & 0xFF);
    
}

//* Global Functions
void init_logging(uint32_t baudRate)
{
    uint32_t usart2  = 17U;
    uint32_t pa2     = 2U;
    uint32_t pa3     = 3U;
    uint32_t af7     = 7U;
    uint32_t cr1_re  = 2U;
    uint32_t cr1_te  = 3U;
    uint32_t cr1_ue  = 13U;

    // Turn on the USART2 peripheral clock.
    RCC_REGS->APB1ENR |= BIT(usart2);
    (void)RCC_REGS->APB1ENR;

    // Turn on the GPIOA peripheral clock.
    RCC_REGS->AHB1ENR |= BIT(GPIOAEN);
    (void)RCC_REGS->AHB1ENR;

    // PA2 = USART2_TX, PA3 = USART2_RX on this STM32. They both need to be in Alternative Function mode.
    // First clear the existing mode bits so we do not accidentally keep old config then we set.
    GPIOA_REGS->MODER &= ~(TWO_BIT_WIDTH << SHIFT_BY_TWO(pa2));
    GPIOA_REGS->MODER &= ~(TWO_BIT_WIDTH << SHIFT_BY_TWO(pa3));
    GPIOA_REGS->MODER |= ((TWO_BIT_WIDTH & GPIOMODE_ALTERNATE_FUNCTION) << SHIFT_BY_TWO(pa2));
    GPIOA_REGS->MODER |= ((TWO_BIT_WIDTH & GPIOMODE_ALTERNATE_FUNCTION) << SHIFT_BY_TWO(pa3));

    // Select which alternate function each pin should use.
    // PA2 and PA3 live in AFR[0] because that register covers pins 0-7. Each AFR field is 4 bits wide.
    // AF7 is the alternate-function mapping for USART2 on PA2/PA3 - according to page 57 of the datasheet.
    // Like with MODER, clear first, then write AF7.
    GPIOA_REGS->AFR[0] &= ~(FOUR_BIT_WIDTH << SHIFT_BY_FOUR(pa2));
    GPIOA_REGS->AFR[0] &= ~(FOUR_BIT_WIDTH << SHIFT_BY_FOUR(pa3));
    GPIOA_REGS->AFR[0] |= ((FOUR_BIT_WIDTH & af7) << SHIFT_BY_FOUR(pa2));
    GPIOA_REGS->AFR[0] |= ((FOUR_BIT_WIDTH & af7) << SHIFT_BY_FOUR(pa3));

    // BRR tells USART2 how fast to shift bits out/in based on the peripheral clock.
    USART2_REGS->BRR = ((APB1_CLK + (baudRate / 2U)) / baudRate);

    // Start from a known state by clearing CR1.
    USART2_REGS->CR1 = 0;

    // TE = Transmitter Enable. Without this, USART2 is on but cannot drive data onto TX.
    USART2_REGS->CR1 |= BIT(cr1_te);

    // RE = Receiver Enable. Not strictly needed since we are just printing to PC.
    USART2_REGS->CR1 |= BIT(cr1_re);

    // UE = USART Enable. This is the master "turn the peripheral on" bit.
    USART2_REGS->CR1 |= BIT(cr1_ue);

    // Set PA2 output speed to very high speed.
    // UART is an alternate-function signal, but the pin still uses GPIO electrical settings underneath. 
    // Higher speed gives cleaner/faster edge transitions.
    GPIOA_REGS->OSPEEDR |= (TWO_BIT_WIDTH << SHIFT_BY_TWO(pa2));

    // Force PA2 output type to push-pull.
    // For UART TX, push-pull is the normal choice because the pin actively drives both high and low.
    GPIOA_REGS->OTYPER &= ~BIT(pa2);

    // Disable pull-up/pull-down on PA2.
    // TX is being actively driven by the USART hardware, so we do not want an
    // internal resistor biasing the line unnecessarily.
    GPIOA_REGS->PUPDR &= ~(TWO_BIT_WIDTH << SHIFT_BY_TWO(pa2));
}

void prints(char str[], ...)
{
    va_list args;
    va_start(args, str);

    int ith = 0;

    while (str[ith] != '\0')
    {
        char ch = str[ith];
        
        if (ch == '%')
        {
            ch = str[++ith];

            switch (ch)
            {
            case 'd':
            {
                int val = va_arg(args, int);
                char numBuffer[32];

                for (int i = 0; i < 32; i++)
                    numBuffer[i] = ' ';

                if (val == 0)
                {
                    uart_write('0');
                    break;
                }

                int index = 31;
                bool ok = true;

                while (val > 0)
                {
                    int digit = val % 10;
                    val /= 10;

                    numBuffer[index--] = convert_to_character(digit);

                    if (index < 0)
                    {
                        ok = false;
                        break;
                    }
                }

                if (!ok)
                    continue;

                for (int i = 0; i < 32; i++)
                {
                    if (is_number(numBuffer[i]))
                        uart_write(numBuffer[i]);
                }
            }
                break;
            
            case 'l':
            {
                ch = str[++ith];

                switch (ch)
                {
                case 'd':
                {
                    long val = va_arg(args, long);
                    char numBuffer[32];

                    for (int i = 0; i < 32; i++)
                        numBuffer[i] = ' ';

                    if (val == 0)
                    {
                        uart_write('0');
                        break;
                    }

                    if (val < 0)
                    {
                        uart_write('-');
                        val = -val;
                    }

                    int index = 31;
                    bool ok = true;

                    while (val > 0)
                    {
                        int digit = (int)(val % 10);
                        val /= 10;

                        numBuffer[index--] = convert_to_character(digit);

                        if (index < 0)
                        {
                            ok = false;
                            break;
                        }
                    }

                    if (!ok)
                        continue;

                    for (int i = 0; i < 32; i++)
                    {
                        if (is_number(numBuffer[i]))
                            uart_write(numBuffer[i]);
                    }
                }
                    break;

                default:
                    uart_write('%');
                    uart_write('l');
                    uart_write(ch);
                    break;
                }
            }
                break;

            case 's':
            {
                const char* val = va_arg(args, char*);
                for (int i = 0; val[i] != '\0'; i++)
                    uart_write(val[i]);
            }
                break;
            }
        }
        else
        {
            uart_write(ch);
        }
        
        ith++;
    }

    va_end(args);
}
