/**
 * @file main.c
 * @brief Entry point and fixed-period scheduler implementation.
 *
 * Implements a deterministic control loop using SysTick as a time base.
 * Handles scheduling, deadline detection (overrun), and recovery policy.
 * Demonstrates behavior under normal operation and injected workload.
 *
 * Written by Edwin J Martinez
 */
#include "hardware.h"
#include "logger.h"
#include "global.h"
#include "sensor.h"
#include <stdbool.h>

static volatile uint32_t g_ms_ticks;
static uint32_t overrun_count = 0;
static int32_t  worst_lateness_ms = 0;
static const uint32_t interval_ms = 10U;
/* Resync Demo policy:
    false = preserve fixed schedule even after missed deadlines (system may remain behind)
    true  = resync after overrun to demonstrate recovery behavior - breaks determinism
*/
static const bool g_resync_on_overrun = true;
// Enable to intentionally overload the loop for testing
static const bool g_force_overrun_test = false;

// ISR Function
void SysTick_Handler(void)
{
    g_ms_ticks++;
}

static void init_timer()
{
    STK_LOAD = (SYS_FREQ / 1000) - 1; // Set the Reload Value to a value that equals 1ms intervals(0.001)
    STK_VAL = 0; // Clear current value and count flag
    STK_CTRL = BIT(2) | BIT(1) | BIT(0); // CLKSOURCE = processor clock(internal), TICKINT = enable interrupt, ENABLE = start clock
}


int main()
{
    init_logging(115200);
    init_timer();

    uint32_t next_run = g_ms_ticks + interval_ms;

    while(1)
    {
        int32_t lateness_ms = (int32_t)(g_ms_ticks - next_run);
        if (lateness_ms >= 0)
        {
            /* OVERRUN Condition Information
                An overrun means the control loop exceeded its time budget. That can happen because the work 
                itself took too long or because the CPU was delayed by interrupt activity or other processing.

                The system maintains deterministic timing under normal conditions, but on overrun it 
                resynchronizes to prevent drift, trading strict periodicity for stability.

            *   To Test:
                - Set g_force_overrun_test to true
            */
            if (lateness_ms > 0) 
            {

                overrun_count++;

                if (lateness_ms > worst_lateness_ms)
                    worst_lateness_ms = lateness_ms;

                prints("\r\n*** [OVERRUN] late by %d ms (count=%d worst=%d)\r\n",
                    (int)lateness_ms,
                    (int)overrun_count,
                    (int)worst_lateness_ms);

                if (g_resync_on_overrun)
                {
                    /*
                        Resync to current time instead of preserving the old schedule.
                        This avoids accumulated drift, but it breaks strict periodic alignment
                        for this cycle.
                    */
                    next_run = g_ms_ticks + interval_ms;
                }
                else
                {
                    /*
                        Preserve the original schedule. This keeps deterministic timing,
                        but if the system is overloaded it may remain behind.
                    */
                    next_run += interval_ms;
                }
            }
            else
            {
                next_run += interval_ms;
            }

            if (g_force_overrun_test)
            {
                simulate_extra_work();
            }
            run_control_cycle();
        }
    }

}
