/* Copyright (c) 2013 Nordic Semiconductor. All Rights Reserved.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the license.txt file.
 */
#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "nrf_gpio.h"
#include "boards.h"

#define BTN_PRESSED 0
#define BTN_RELEASED 1

int main(void)
{
    // Configure BUTTON0 as a regular input
    nrf_gpio_cfg_input(BUTTON_0, NRF_GPIO_PIN_NOPULL);
    
    // Configure BUTTON1 with SENSE enabled (not possible using nrf_gpio.h alone)
    nrf_gpio_cfg_sense_input(BUTTON_1, NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_SENSE_LOW);
    
    // Configure the LED pins as outputs
    nrf_gpio_range_cfg_output(LED_START, LED_STOP);
    
    nrf_gpio_pin_set(LED_0);
    
    // Configure the RAM retention parameters
    NRF_POWER->RAMON = POWER_RAMON_ONRAM0_RAM0On   << POWER_RAMON_ONRAM0_Pos
                     | POWER_RAMON_ONRAM1_RAM1On   << POWER_RAMON_ONRAM1_Pos
                     | POWER_RAMON_OFFRAM0_RAM0Off << POWER_RAMON_OFFRAM0_Pos
                     | POWER_RAMON_OFFRAM1_RAM1Off << POWER_RAMON_OFFRAM1_Pos;

    // Set the GPIOTE PORT event as interrupt source, and enable interrupts for GPIOTE
    NRF_GPIOTE->INTENSET = GPIOTE_INTENSET_PORT_Msk;
    NVIC_EnableIRQ(GPIOTE_IRQn);
    
    while(1)
    {     
        // If BUTTON0 is pressed..
        if(nrf_gpio_pin_read(BUTTON_0) == BTN_PRESSED)
        {
            nrf_gpio_pin_clear(LED_0);
            
            // Enter CONSTLAT mode if desired, otherwise LOWPWR mode will be used (LOWPWR is recommended for most applications)
            //NRF_POWER->TASKS_CONSTLAT = 1;
            
            // Enter System ON
            __WFE();
        }
    }
}

void GPIOTE_IRQHandler(void)
{
    // This handler will be run after wakeup from system ON (GPIO wakeup)
    if(NRF_GPIOTE->EVENTS_PORT)
    {
        NRF_GPIOTE->EVENTS_PORT = 0;
        
        nrf_gpio_pin_toggle(LED_0);
    }
}
