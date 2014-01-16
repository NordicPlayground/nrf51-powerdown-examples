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
#include "nrf_nvmc.h"
#include "boards.h"

#define BTN_PRESSED 0
#define BTN_RELEASED 1

#define FLASH_DATA_START 0x10000
#define FLASH_DATA_END   0x30000

void gpio_init()
{
    nrf_gpio_range_cfg_input(0, 6, NRF_GPIO_PIN_NOPULL);

    // Configure BUTTON7 as an active low wakeup source 
    NRF_GPIO->PIN_CNF[BUTTON_7] = (GPIO_PIN_CNF_SENSE_Low << GPIO_PIN_CNF_SENSE_Pos)
                               | (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)
                               | (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)
                               | (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
                               | (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos);  
    
    // Configure the LED pins as outputs
    nrf_gpio_range_cfg_output(LED_START, LED_STOP);      
}
uint8_t nrf_nvmc_read_byte(uint32_t flash_address)
{
    return (uint8_t)(*((uint32_t*)(flash_address & 0xFFFFFFFC)) >> ((flash_address & 0x03) *8));
}
void start_32k_and_rtc()
{
    NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_LFCLKSTART = 1;
    while(NRF_CLOCK->EVENTS_LFCLKSTARTED == 0);
    NRF_RTC1->TASKS_CLEAR = 1;
    NRF_RTC1->TASKS_START = 1;    
}
/**
 * main function
 * \return 0. int return type required by ANSI/ISO standard. 
 */
int main(void)
{
    uint8_t counter;
    uint32_t flash_addr;
    gpio_init();
 
    while(nrf_gpio_pin_read(BUTTON_0) == BTN_PRESSED);
    
    // Internal 32kHz RC
    NRF_CLOCK->LFCLKSRC = CLOCK_LFCLKSRC_SRC_Xtal;
    
    // Configure the RTC to run at 4 second intervals, and make sure COMPARE0 generates an interrupt (this will be the wakeup source in system ON)
    NRF_RTC1->PRESCALER = 1;
    NRF_RTC1->EVTENSET = RTC_EVTEN_COMPARE0_Msk; 
    NRF_RTC1->INTENSET = RTC_INTENSET_COMPARE0_Msk; 
    NRF_RTC1->CC[0] = 8 * 32768;
    NVIC_EnableIRQ(RTC1_IRQn);
    NRF_GPIOTE->INTENSET = GPIOTE_INTENSET_PORT_Msk;
    NVIC_EnableIRQ(GPIOTE_IRQn);
    
    while(1)
    {     
        
        for(flash_addr = FLASH_DATA_START; flash_addr < FLASH_DATA_END; flash_addr++)
        {
            if(nrf_nvmc_read_byte(flash_addr) == 0xFF)
            {
                break;
            }
        }
        counter = nrf_nvmc_read_byte(flash_addr-1);
        counter++;
        if(counter > 8)
        {
            counter = 0;
        }    
        nrf_nvmc_write_byte(flash_addr, counter);
        //counter = NRF_POWER->GPREGRET + 1;  
        NRF_GPIO->OUTCLR = 0xFF << 8;
        NRF_GPIO->OUTSET = counter << 9 | 0x100;
        nrf_delay_us(2000000);
        //while(nrf_gpio_pin_read(BUTTON7) == BTN_RELEASED);
        //while(nrf_gpio_pin_read(BUTTON7) == BTN_PRESSED);   
        NRF_GPIO->OUTCLR = 0xFF << 8;    
        switch(counter)
        {
            case 0:
                // System OFF, no RAM retention
                NRF_POWER->RAMON = 0;
                NRF_POWER->SYSTEMOFF = 1;
                break;
            case 1:
                // System OFF, 8 kB RAM retention
                NRF_POWER->RAMON = POWER_RAMON_OFFRAM0_Msk;
                // Enter system OFF mode
                NRF_POWER->SYSTEMOFF = 1;
                break;            
            case 2:
                // System OFF, 16 kB RAM retention
                NRF_POWER->RAMON = POWER_RAMON_OFFRAM0_Msk | POWER_RAMON_OFFRAM1_Msk;
                // Enter system OFF mode
                NRF_POWER->SYSTEMOFF = 1;
                break;
            case 3:
                // System ON, 32k and RTC off, no RAM retention
                NRF_POWER->RAMON = 0;
                __WFI();
                break;
            case 4:
                // System ON, 32k and RTC off, 8 kB RAM retention
                NRF_POWER->RAMON = POWER_RAMON_ONRAM0_Msk;
                __WFI();
                break;
            case 5:
                // System ON, 32k and RTC off, 16 kB RAM retention
                NRF_POWER->RAMON = POWER_RAMON_ONRAM0_Msk | POWER_RAMON_ONRAM1_Msk;
                __WFI();
                break;
            case 6:
                // System ON, 32k and RTC running, no RAM retention
                start_32k_and_rtc();
                NRF_POWER->RAMON = 0;
                __WFI();
                break;
            case 7:
                // System ON, 32k and RTC running, 8 kB RAM retention
                start_32k_and_rtc();
                NRF_POWER->RAMON = POWER_RAMON_ONRAM0_Msk;
                __WFI();
                break;
            case 8:
                // System ON, 32k and RTC running, 16 kB RAM retention
                start_32k_and_rtc();
                NRF_POWER->RAMON = POWER_RAMON_ONRAM0_Msk | POWER_RAMON_ONRAM1_Msk;
                __WFI();              
                break;        
            default:
                counter = 0;
                break;
        }
    }
}

void RTC1_IRQHandler(void)
{
    // This handler will be run after wakeup from system ON (RTC wakeup)
    if(NRF_RTC1->EVENTS_COMPARE[0])
    {
        NRF_RTC1->EVENTS_COMPARE[0] = 0;
        NRF_RTC1->TASKS_STOP = 1;
        NRF_RTC1->TASKS_CLEAR = 1;
        NRF_CLOCK->TASKS_LFCLKSTOP = 1;
    }
}
void GPIOTE_IRQHandler(void)
{
    NRF_GPIOTE->EVENTS_PORT = 0;
}

/**
 *@}
 **/
