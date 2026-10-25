
/*
 * File:   main.c
 * Author: bgofman
 *
 * ECE118 Lab 3 - DC Motor
 *
 * Part 2: Unidirectional DC Motor (DS3658)
 * Part 3: Snubbing Inductive Kickback (DS3658)
 * Part 4: Bidirectional DC Motor (L298)
 *
 * Uno32 I/O Board Connections:
 *
 * Potentiometer (Parts 2, 3, 4):
 *   W03  - Signal (middle pin).
 *   3.3V - Power.
 *   GND  - Ground.
 *
 * PWM Output (Parts 2, 3, 4):
 *   Z06  - PWM signal.
 *
 * DS3658 (Parts 2, 3):
 *   Z06 -> J2 pin 3 (Input A).
 *   J1 pin 1 (Output A) -> Motor negative.
 *   Motor positive -> Motor supply (+).
 *   J4 pin 1 -> Motor supply (+).
 *   J4 pin 2 -> Ground.
 *   J3 CLOSED for clamped mode.
 *
 * L298 (Part 4):
 *   Z06 -> CON2 pin 1 (ENA).
 *   Z03 -> CON2 pin 2 (IN1).
 *   Z05 -> CON2 pin 3 (IN2).
 *   Z07 -> Direction switch.
 *   Motor -> OUT1 and OUT2.
 *   POWER pin 1 -> Motor supply (+).
 *   POWER pin 2 -> Ground.
 *
 * LED Bar:
 *   Built into the Uno32 I/O Board.
 *
 * All grounds must be connected together.
 */

#define LAB_PART 2    // Select 2 or 3 for DS3658, 4 for L298.
#define ADC_MAX 1023 // AD_ReadADPin() returns a 10-bit value (0-1023).

#include <BOARD.h>     // Initialize Uno32.
#include <AD.h>        // Read potentiometer.
#include <LED.h>       // Control LED bar.
#include <pwm.h>       // Generate PWM signal.

#if LAB_PART == 4
#include <IO_Ports.h>  // Part 4: Read switch and control L298 direction.
#endif

int main(void)
{
    // Initialize Uno32.
    BOARD_Init();

    // PDF Parts 2, 3, 4: Display motor speed on the Uno32 LED bar.
    // Initialize all three LED banks.
    LED_Init();
    LED_AddBanks(LED_BANK1 | LED_BANK2 | LED_BANK3);

    // PDF Parts 2, 3, 4: Read the potentiometer through an analog input.
    // Configure W03 as the potentiometer input.
    AD_AddPins(AD_PORTW3);

    // PDF Parts 2, 3, 4: Generate PWM to control motor speed.
    // Initialize PWM at the default frequency (1 kHz).
    PWM_Init();
    PWM_AddPins(PWM_PORTZ06);

#if LAB_PART == 4

    // PDF Part 4: The switch determines motor direction.
    // Configure Z07 as the switch input.
    PORTZ07_TRIS = 1;

    // PDF Part 4: Control L298 direction using IN1 and IN2.
    // Configure Z03 and Z05 as outputs.
    PORTZ03_TRIS = 0;
    PORTZ05_TRIS = 0;

    // Set the initial motor direction.
    PORTZ03_LAT = 1;
    PORTZ05_LAT = 0;

#endif

    while (1)
    {
        // Check for new ADC data.
        if (AD_IsNewDataReady())
        {
            // PDF Parts 2, 3, 4: Read potentiometer from W03.
            unsigned int potValue = AD_ReadADPin(AD_PORTW3);

            // Check that the ADC value is valid.
            if (potValue <= ADC_MAX)
            {
                // PDF Parts 2, 4: Map 0V to 0% and 3.3V to 100% PWM.
                // MAX_PWM = 1000 (100% duty cycle).
                unsigned int dutyCycle = potValue * MAX_PWM / ADC_MAX;

#if LAB_PART == 4

                // PDF Part 4: Read the switch to determine direction.
                // HIGH = Forward, LOW = Reverse.
                unsigned int direction = PORTZ07_BIT;

                // PDF Part 4: Control direction using L298 IN1 and IN2.
                // IN1 and IN2 must have opposite values.
                PORTZ03_LAT = direction;
                PORTZ05_LAT = !direction;

#endif

                // PDF Parts 2, 3, 4: Control motor speed using PWM.
                // Send PWM signal through Z06.
                PWM_SetDutyCycle(PWM_PORTZ06, dutyCycle);

#if LAB_PART == 4

                // PDF Part 4: LEDs indicate motor speed and direction.
                // Use 8 LEDs for speed and Bank 3 for direction.
                unsigned int ledCount = potValue * 8 / ADC_MAX;

#else

                // PDF Parts 2, 3: Display potentiometer value.
                // Convert ADC value to 0-12 LEDs.
                unsigned int ledCount = potValue * 12 / ADC_MAX;

#endif

                // Create the LED pattern.
                unsigned int ledPattern = (1U << ledCount) - 1U;

                // Update LED Banks 1 and 2.
                LED_SetBank(LED_BANK1, ledPattern & 0x0F);
                LED_SetBank(LED_BANK2, (ledPattern >> 4) & 0x0F);

#if LAB_PART == 4

                // PDF Part 4: Display motor direction on LED Bank 3.
                // First LED = Forward, last LED = Reverse.
                LED_SetBank(LED_BANK3, direction ? 0x01 : 0x08);

#else

                // PDF Parts 2, 3: Display speed on LED Bank 3.
                LED_SetBank(LED_BANK3, (ledPattern >> 8) & 0x0F);

#endif
            }
        }
    }

    return 0;
}
