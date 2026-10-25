/* 
 * File:   main.c
 * Author: bgofman
 *
 * Created on October 24, 2026, 1:02 PM
 */

#include <xc.h>
#include <BOARD.h>
#include "Stepper.h"

int main(void)
{
    // Initialize Uno32
    BOARD_Init();

    // Initialize stepper motor
    Stepper_Init();

    // Set speed: 100 steps per second
    Stepper_SetRate(100);

    // Move forward 200 steps
    Stepper_InitSteps(FORWARD, 200);

    while (1)
    {
        // Main loop
    }

    return 0;
}


