/**
 * @file global.h
 * @brief Global macros and shared definitions.
 *
 * Contains utility macros, bit manipulation helpers, and shared
 * constants used across the project.
 *
 * Written by Edwin J Martinez
 */

#pragma once

//* Helpers
#define BIT(p)              (0x01UL << (p))
#define SHIFT_BY_TWO(p)     ((p) * 0x02U)
#define SHIFT_BY_FOUR(p)    ((p) * 0x04U)
#define TWO_BIT_WIDTH       0x3UL
#define FOUR_BIT_WIDTH      0x0FUL
