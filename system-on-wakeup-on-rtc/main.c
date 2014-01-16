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
    
    // Configure BUTTON1 with SENSE enabled (not possible using nrf_gpio.h)
    NRF_GPIO->PIN_CNF[BUTTON1] = (GPIO_PIN_CNF_SENSE_Low << GPIO_PIN_CNF_SENSE_Pos)
                               | (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)
                               | (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)
                               | (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
                               | (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos);
    
    // Configure the LED pins as outputs
    nrf_gpio_range_cfg_output(LED_START, LED_STOP);
    
    nrf_gpio_pin_set(LED0);

    // Internal 32kHz RC
    NRF_CLOCK->LFCLKSRC = CLOCK_LFCLKSRC_SRC_RC << CLOCK_LFCLKSRC_SRC_Pos;
    
    // Start the 32 kHz clock, and wait for the start up to complete
    NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_LFCLKSTART = 1;
    while(NRF_CLOCK->EVENTS_LFCLKSTARTED == 0);
    
    // Configure the RTC to run at 2 second intervals, and make sure COMPARE0 generates an interrupt (this will be the wakeup source)
    NRF_RTC1->PRESCALER = 3277;
    NRF_RTC1->EVTENSET = RTC_EVTEN_COMPARE0_Msk; 
    NRF_RTC1->INTENSET = RTC_INTENSET_COMPARE0_Msk; 
    NRF_RTC1->CC[0] = 2*10;
    NRF_RTC1->TASKS_START = 1;
    NVIC_EnableIRQ(RTC1_IRQn);
            
    // Configure the RAM retention parameters
    NRF_POWER->RAMON = POWER_RAMON_ONRAM0_RAM0On   << POWER_RAMON_ONRAM0_Pos
                     | POWER_RAMON_ONRAM1_RAM1Off  << POWER_RAMON_ONRAM1_Pos
                     | POWER_RAMON_OFFRAM0_RAM0Off << POWER_RAMON_OFFRAM0_Pos
                     | POWER_RAMON_OFFRAM1_RAM1Off << POWER_RAMON_OFFRAM1_Pos;
    
    while(1)
    {     
        // If BUTTON0 is pressed..
        if(nrf_gpio_pin_read(BUTTON0) == BTN_PRESSED)
        {
            nrf_gpio_pin_clear(LED0);
                      
            // Keep entering sleep mode as long as BUTTON1 is released
            while(nrf_gpio_pin_read(BUTTON1) == BTN_RELEASED)
            {
                // Enter system ON. After wakeup the chip will not reset, and the MCU will continue in on the next line of the code
                __WFE();
            }
            
            nrf_gpio_pin_set(LED0);
            nrf_gpio_pin_clear(LED1);
        }
    }
}

void RTC1_IRQHandler(void)
{
    // This handler will be run after wakeup from system ON (RTC wakeup)
    if(NRF_RTC1->EVENTS_COMPARE[0])
    {
        NRF_RTC1->EVENTS_COMPARE[0] = 0;
        
        nrf_gpio_pin_toggle(LED1);
        
        NRF_RTC1->TASKS_CLEAR = 1;
    }
}

