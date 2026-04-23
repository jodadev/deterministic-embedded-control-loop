/**
 * @file logger.h
 * @brief Lightweight formatted logging interface over USART.
 *
 * Provides a minimal printf-style logging function supporting basic
 * format specifiers for debugging and runtime visibility.
 *
 * Written by Edwin J Martinez
 */

#pragma once
#include <stdint.h>
#include <stdarg.h>

/// @brief Initializes the UART/USART to enable logging/debugging to the PC monitor.
/// @param baudRate ensure that this value matches the value set on your PC monitor for accurate communication.
void init_logging(uint32_t baudRate);

/// @brief Print to the console. Currently only supports string(%s), integer(%d) and long integer(%ld) formatting.
/// @param str String with optional formatting flags.
/// @param ... list of arguments.
void prints(char str[], ...);
