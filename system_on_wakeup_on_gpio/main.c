/* Copyright (c) 2009 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/*
* Simple example that shows how to enter System-On low power mode.
* Press Button 1 to enter System-On low power mode.
* Press Button 2 to exit System-On low power mode.
* This example does not use a softdevice.
*
*/

#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "nrf_gpio.h"
#include "boards.h"

#define BTN_PRESSED 0
#define BTN_RELEASED 1

#define NRF52_ONRAM1_OFFRAM1  	POWER_RAM_POWER_S0POWER_On      << POWER_RAM_POWER_S0POWER_Pos      \
												      | POWER_RAM_POWER_S1POWER_On      << POWER_RAM_POWER_S1POWER_Pos      \
												      | POWER_RAM_POWER_S0RETENTION_On  << POWER_RAM_POWER_S0RETENTION_Pos  \
	                            | POWER_RAM_POWER_S1RETENTION_On  << POWER_RAM_POWER_S1RETENTION_Pos; 
												
#define NRF52_ONRAM1_OFFRAM0    POWER_RAM_POWER_S0POWER_On      << POWER_RAM_POWER_S0POWER_Pos      \
												      | POWER_RAM_POWER_S1POWER_On      << POWER_RAM_POWER_S1POWER_Pos      \
												      | POWER_RAM_POWER_S0RETENTION_Off << POWER_RAM_POWER_S0RETENTION_Pos  \
	                            | POWER_RAM_POWER_S1RETENTION_Off << POWER_RAM_POWER_S1RETENTION_Pos;														
												
#define NRF52_ONRAM0_OFFRAM0    POWER_RAM_POWER_S0POWER_Off     << POWER_RAM_POWER_S0POWER_Pos      \
												      | POWER_RAM_POWER_S1POWER_Off     << POWER_RAM_POWER_S1POWER_Pos;
												
void configure_ram_retention(void)
{
#ifdef NRF51    
    // Configure nRF51 RAM retention parameters. Set for System On 16kB RAM retention
    NRF_POWER->RAMON = POWER_RAMON_ONRAM0_RAM0On   << POWER_RAMON_ONRAM0_Pos
                     | POWER_RAMON_ONRAM1_RAM1On   << POWER_RAMON_ONRAM1_Pos
                     | POWER_RAMON_OFFRAM0_RAM0Off << POWER_RAMON_OFFRAM0_Pos
                     | POWER_RAMON_OFFRAM1_RAM1Off << POWER_RAMON_OFFRAM1_Pos;
		NRF_POWER->RAMONB = POWER_RAMONB_ONRAM2_RAM2Off   << POWER_RAMONB_ONRAM2_Pos
                      | POWER_RAMONB_ONRAM3_RAM3Off   << POWER_RAMONB_ONRAM3_Pos	
	                    | POWER_RAMONB_OFFRAM2_RAM2Off  << POWER_RAMONB_OFFRAM2_Pos
	                    | POWER_RAMONB_OFFRAM3_RAM3Off  << POWER_RAMONB_OFFRAM3_Pos;
#endif //NRF51

#ifdef NRF52
		// Configure nRF52 RAM retention parameters. Set for System On 64kB RAM retention
		NRF_POWER->RAM[0].POWER = NRF52_ONRAM1_OFFRAM0;
		NRF_POWER->RAM[1].POWER = NRF52_ONRAM1_OFFRAM0;
		NRF_POWER->RAM[2].POWER = NRF52_ONRAM1_OFFRAM0;
		NRF_POWER->RAM[3].POWER = NRF52_ONRAM1_OFFRAM0;
		NRF_POWER->RAM[4].POWER = NRF52_ONRAM1_OFFRAM0;
		NRF_POWER->RAM[5].POWER = NRF52_ONRAM1_OFFRAM0;
		NRF_POWER->RAM[6].POWER = NRF52_ONRAM1_OFFRAM0;
		NRF_POWER->RAM[7].POWER = NRF52_ONRAM1_OFFRAM0;	
#endif //NRF52	
}


/**
 * @brief Function for application main entry.
 */
int main(void)
{
    // Configure BUTTON0 as a regular input
    nrf_gpio_cfg_input(BUTTON_1, NRF_GPIO_PIN_PULLUP);
    
    // Configure BUTTON 2 with SENSE enabled so that CPU is enabled (exit System-On low power mode) when pressing Button 2
    nrf_gpio_cfg_sense_input(BUTTON_2, NRF_GPIO_PIN_PULLUP, NRF_GPIO_PIN_SENSE_LOW);
    
    // Configure the LED pins as outputs
    nrf_gpio_range_cfg_output(LED_START, LED_START);
    
		// Turn on LED 1
    nrf_gpio_pin_clear(LED_1);
	
		//Configure RAM retention. More RAM retention means increased current consumption (see electrical specification in the Product Specification, power chapter)
		configure_ram_retention();

    // Set the GPIOTE PORT event as interrupt source, and enable interrupts for GPIOTE
    NRF_GPIOTE->INTENSET = GPIOTE_INTENSET_PORT_Msk;
    NVIC_EnableIRQ(GPIOTE_IRQn);
    
    while(1)
    {     
        // If BUTTON 1 is pressed..
        if(nrf_gpio_pin_read(BUTTON_1) == BTN_PRESSED)
        {
            nrf_gpio_pin_set(LED_1);
            
            // Enter CONSTLAT mode if desired, otherwise LOWPWR mode will be used (LOWPWR is recommended for most applications)
            //NRF_POWER->TASKS_CONSTLAT = 1;
            
            // Enter System ON sleep mode
            __WFE();
            __SEV();
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
        
        nrf_gpio_pin_clear(LED_1);
    }
}

/** @} */
