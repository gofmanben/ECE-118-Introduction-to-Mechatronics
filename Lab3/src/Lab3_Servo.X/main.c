
/*
 * File:   main.c
 * Author: bgofman
 *
 * ECE118 Lab 3 - Part 1: RC Servo
 *
 * Uno32 I/O Board Connections:
 *
 * Potentiometer:
 *   W03  - Signal (middle pin).
 *   3.3V - Power.
 *   GND  - Ground.
 *
 * RC Servo:
 *   X03  - Signal (yellow wire).
 *   5V   - Power (regulated 5V).
 *   GND  - Ground (brown wire).
 *
 * LED Bar:
 *   Built into the Uno32 I/O Board.
 *   No external wiring required.
 *
 * All grounds must be connected together.
 */

#define ADC_MAX 1023  // AD_ReadADPin() returns a 10-bit value (0-1023).

#include <BOARD.h>     // Initialize Uno32.
#include <AD.h>        // Read potentiometer.
#include <LED.h>       // Control LED bar.
#include <RC_Servo.h>  // Control RC servo.

int main(void)
{
    // Initialize Uno32.
    BOARD_Init();

    // PDF Part 1: Display the potentiometer value on the Uno32 LED bar.
    // Initialize all three LED banks.
    LED_Init();
    LED_AddBanks(LED_BANK1 | LED_BANK2 | LED_BANK3);

    // PDF Part 1: Read the potentiometer through an analog input.
    // Configure W03 as the potentiometer input.
    AD_AddPins(AD_PORTW3);

    // PDF Part 1: Control the RC servo using a pulse-width signal.
    // Initialize the servo output on X03.
    RC_Init();
    RC_AddPins(RC_PORTX03);

    while (1)
    {
        // Check for new ADC data.
        if (AD_IsNewDataReady())
        {
            // PDF Part 1: Read the potentiometer through an analog input.
            // Read the potentiometer from W03 (0-1023).
            unsigned int potValue = AD_ReadADPin(AD_PORTW3);

            // Check that the ADC value is valid.
            if (potValue <= ADC_MAX)
            {
                // PDF Part 1: Map the potentiometer reading to the servo pulse width.
                // The servo should move approximately +/-40 to +/-60 degrees from center.
                // Do not change MINPULSE or MAXPULSE in RC_Servo.h.
                unsigned int pulseTime = MINPULSE + (potValue * (MAXPULSE - MINPULSE)) / ADC_MAX;

                // PDF Part 1: Move the servo according to the potentiometer position.
                // Send the calculated pulse width to the servo on X03.
                RC_SetPulseTime(RC_PORTX03, pulseTime);

                // PDF Part 1: Display a scaled potentiometer value on the LED bar.
                // Convert the ADC value to the number of LEDs (0-12).
                unsigned int ledCount = (potValue * 12) / ADC_MAX;

                // Create the LED pattern.
                unsigned int ledPattern = (1U << ledCount) - 1U;

                // PDF Part 1: Update the Uno32 LED bar.
                // Display the potentiometer value on all three LED banks.
                LED_SetBank(LED_BANK1, ledPattern & 0x0F);
                LED_SetBank(LED_BANK2, (ledPattern >> 4) & 0x0F);
                LED_SetBank(LED_BANK3, (ledPattern >> 8) & 0x0F);
            }
        }
    }

    return 0;
}
