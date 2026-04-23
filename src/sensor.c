/**
 * @file sensor.c
 * @brief Implementation of simulated sensor control loop.
 *
 * Simulates memory-mapped sensor hardware, including controlled fault
 * injection, validation of sensor data, and recovery using the last
 * known good value. Designed to be executed at a fixed interval by a
 * scheduler.
 *
 * Written by Edwin J Martinez
 */

#include "sensor.h"
#include "logger.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// Software-backed stand-ins for memory-mapped hardware registers
static volatile uint32_t SENSOR_DATA_REG = 50;     // simulated sensor value
static volatile uint32_t SENSOR_STATUS_REG = 0;    // 0 = OK, 1 = FAULT

static uint32_t last_known_good = 50;
static uint32_t cycle_count = 0;

static uint32_t sensor_read(void)
{
    return SENSOR_DATA_REG;
}

static bool sensor_is_valid(uint32_t value)
{
    // Check status register first
    if (SENSOR_STATUS_REG != 0U)
    {
        return false;
    }

    // Range validation
    if (value > 100U)
    {
        return false;
    }

    return true;
}


void run_control_cycle(void)
{
    cycle_count++;

    // Controlled fault injection for testing validation and recovery
    if ((cycle_count % 25U) == 0U)
    {
        SENSOR_DATA_REG = 999U;        // invalid value
        SENSOR_STATUS_REG = 1U;        // flag fault
    }
    else
    {
        SENSOR_DATA_REG = (cycle_count % 100U); // normal changing data
        SENSOR_STATUS_REG = 0U;
    }

    // Read simulated sensor register
    uint32_t value = sensor_read();

    // Validate sensor data
    if (sensor_is_valid(value))
    {
        last_known_good = value;

        prints("[OK] value=%d\r\n", value);
    }
    else
    {
        prints("[FAULT] invalid reading (%d)\r\n", value);

        // Fallback to last known good value
        value = last_known_good;

        prints("[RECOVERY] using last_known_good=%d\r\n", value);
    }
}

void simulate_extra_work(void)
{
    volatile uint32_t dummy = 0;

    for (uint32_t i = 0; i < 10000U; i++)
    {
        dummy += i;
    }

    (void)dummy;
}
