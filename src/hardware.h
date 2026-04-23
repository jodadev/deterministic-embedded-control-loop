/**
 * @file hardware.h
 * @brief Hardware register definitions and memory mappings.
 *
 * Defines base addresses, register structures, and macros for
 * interacting with MCU peripherals such as GPIO, RCC, USART, and SysTick.
 *
 * Written by Edwin J Martinez
 */

#pragma once

#include <stdint.h>

//* Base Addresses
#define PERIPH_BASE         0x40000000UL
#define AHB1_BASE           (PERIPH_BASE + 0x00020000UL)
#define APB1_BASE           (PERIPH_BASE + 0x00000000UL)
#define RCC_BASE            (AHB1_BASE   + 0x00003800UL)
#define GPIOA_BASE          (AHB1_BASE   + 0x00000000UL)
#define GPIOB_BASE          (AHB1_BASE   + 0x00000400UL)
#define USART2_BASE         (APB1_BASE   + 0x00004400UL)

#define SYS_FREQ            16000000
#define APB1_CLK            (SYS_FREQ)

//* SYSTICK
#define STK_CTRL            *((volatile uint32_t*) 0xE000E010)
#define STK_LOAD            *((volatile uint32_t*) 0xE000E014)
#define STK_VAL             *((volatile uint32_t*) 0xE000E018)
#define STK_CALIB           *((volatile uint32_t*) 0xE000E01C)

//* RCC clock bits
#define GPIOAEN             0x00U
#define GPIOBEN             0x01U

//* GPIO Register Layout
typedef struct
{
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFR[2];
} GPIORegisters;

//* RCC Register Layout
typedef struct
{
    volatile uint32_t CR;
    volatile uint32_t PLL;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t AHB1RSTR;
    volatile uint32_t AHB2RSTR;
    volatile uint32_t AHB3RSTR;
    volatile uint32_t DUMMY_1;
    volatile uint32_t APB1RSTR;
    volatile uint32_t APB2RSTR;
    volatile uint32_t DUMMY_2[2];
    volatile uint32_t AHB1ENR;
    volatile uint32_t AHB2ENR;
    volatile uint32_t AHB3ENR;
    volatile uint32_t DUMMY_3;
    volatile uint32_t APB1ENR;
    volatile uint32_t APB2ENR;
    volatile uint32_t DUMMY_4[2];
    volatile uint32_t AHB1LPENR;
    volatile uint32_t AHB2LPENR;
    volatile uint32_t AHB3LPENR;
    volatile uint32_t DUMMY_5;
    volatile uint32_t APB1LPENR;
    volatile uint32_t APB2LPENR;
    volatile uint32_t DUMMY_6[2];
    volatile uint32_t BDCR;
    volatile uint32_t CSR;
    volatile uint32_t DUMMY_7[2];
    volatile uint32_t SSCGR;
    volatile uint32_t PLLI2SCFGR;
    volatile uint32_t PLLSAICFGR;
    volatile uint32_t DCKCFGR;
    volatile uint32_t CKGATENR;
    volatile uint32_t DCKCFGR2;
} RCCRegisters;

//* USART Register Layout
typedef struct
{
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t BRR;
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t GTPR;
} UARTRegisters;

typedef enum
{
    GPIOMODE_INPUT              = 0x0U,
    GPIOMODE_OUTPUT             = 0x1U,
    GPIOMODE_ALTERNATE_FUNCTION = 0x2U,
    GPIOMODE_ANALOG             = 0x3U
} GPIOMode;

typedef enum
{
    NONE        = 0x0U,
    PULL_UP     = 0x1U,
    PULL_DOWN   = 0x2U
} PUPDMode;


//* Peripheral instances
#define GPIOA_REGS          ((GPIORegisters*) GPIOA_BASE)
#define GPIOB_REGS          ((GPIORegisters*) GPIOB_BASE)

//* RCC instances
#define RCC_REGS            ((RCCRegisters*) RCC_BASE)

//* USART2 instances
#define USART2_REGS         ((UARTRegisters*) USART2_BASE)
