/**
 * @file sensor.h
 * @brief Interface for simulated sensor control loop and fault injection.
 *
 * Provides the public API for executing a periodic sensor control cycle
 * with validation and recovery, as well as utilities for injecting
 * controlled workload to test scheduler overrun behavior.
 *
 * Written by Edwin J Martinez
 */

#pragma once

/*
    Executes one iteration of the sensor control loop.

    Intended to be called at a fixed period (e.g., every 10 ms) by the scheduler.

    Responsibilities:
    - inject controlled faults
    - read simulated hardware registers
    - validate data
    - recover using last known good value
*/
void run_control_cycle(void);

/*
    Adds intentional CPU load to the current cycle.

    Used only to force a controlled deadline miss so the scheduler's
    overrun detection and recovery policy can be demonstrated.
*/
void simulate_extra_work(void);
