
/*
 * File:   main.c
 * Author: bgofman
 *
 * ECE118 Lab 3 - Stepper Motor
 * Part 5: L298
 * Part 6: DRV8811
 */

#include <xc.h>
#include <BOARD.h>
#include "Stepper.h"

#define MOTOR_FULL_STEPS 200       // 200 full steps per revolution.
#define MOTOR_HALF_STEPS 400       // 400 half steps per revolution.
#define MOTOR_MICRO_STEPS 1600     // 1600 steps per revolution at 1/8 microstepping.

int main(void)
{
    // Select the number of steps per revolution.
    int32_t steps = MOTOR_FULL_STEPS;

    // Initialize Uno32.
    BOARD_Init();

    // Initialize stepper motor.
    Stepper_Init();

    // Set motor speed to 100 steps per second.
    Stepper_SetRate(100);

    while (1)
    {
        // Move forward.
        Stepper_InitSteps(FORWARD, steps);

        // Wait until the motor stops.
        while (Stepper_IsStepping()) {
        }

        // Move backward.
        Stepper_InitSteps(REVERSE, steps);

        // Wait until the motor stops.
        while (Stepper_IsStepping()) {
        }
    }

    return 0;
}
