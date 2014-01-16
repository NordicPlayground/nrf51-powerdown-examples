/* Copyright (c) 2013 Nordic Semiconductor. All Rights Reserved.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the license.txt file.
 */
#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "boards.h"

#define BTN_PRESSED 0
#define BTN_RELEASED 1

int main(void)
{
    // Configure BUTTON0 as a regular input
    nrf_gpio_cfg_input(BUTTON0, NRF_GPIO_PIN_NOPULL);
    
    // Configure the LED pins as outputs
    nrf_gpio_range_cfg_output(LED_START, LED_STOP);
    
    // Configure BUTTON1 with SENSE enabled (not possible using nrf_gpio.h)
    NRF_GPIO->PIN_CNF[BUTTON1] = (GPIO_PIN_CNF_SENSE_Low << GPIO_PIN_CNF_SENSE_Pos)
                               | (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)
                               | (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)
                               | (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
                               | (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos);
    
    // Configure LED0 as an output
    nrf_gpio_cfg_output(LED0);
    
    // Set LED0
    nrf_gpio_pin_set(LED0);
    
    // If desired, change the RAM retention parameters:
    NRF_POWER->RAMON = POWER_RAMON_ONRAM0_RAM0On   << POWER_RAMON_ONRAM0_Pos
                     | POWER_RAMON_ONRAM1_RAM1On   << POWER_RAMON_ONRAM1_Pos
                     | POWER_RAMON_OFFRAM0_RAM0Off << POWER_RAMON_OFFRAM0_Pos
                     | POWER_RAMON_OFFRAM1_RAM1Off << POWER_RAMON_OFFRAM1_Pos;
    
    while(1)
    {     
        // If BUTTON0 is pressed..
        if(nrf_gpio_pin_read(BUTTON0) == BTN_PRESSED)
        {
            // Clear LED0
            nrf_gpio_pin_clear(LED0);

            // Enter system OFF. After wakeup the chip will be reset, and the MCU will run from the top 
            NRF_POWER->SYSTEMOFF = 1;
        }
    }
}
