nrf51-watchdog-resetreas-example
==================

This example demonstrates the use of the watchdog, the system off power-down mode and the RESETREAS register, that can be used to determine the reset reason (i.e. whether the chip was reset from system off, from power-on, reset by the watchdog or similar. 

See main.c for a detailed description of the application behavior. 

Requirements
------------
- nRF51 SDK version 5.1.0
- nRF51822 Development Kit version 2.1.0 or later

The project may need modifications to work with later versions or other boards. 

To compile it, clone the repository in the nrf51822/Board/nrf6310/ folder.

About this project
------------------
This application is one of several applications that has been built by the support team at Nordic Semiconductor, as a demo of some particular feature or use case. It has not necessarily been thoroughly tested, so there might be unknown issues. It is hence provided as-is, without any warranty. 

However, in the hope that it still may be useful also for others than the ones we initially wrote it for, we've chosen to distribute it here on GitHub. 

The application is built to be used with the official nRF51 SDK, that can be downloaded from www.nordicsemi.no, provided you have a product key for one of our kits.
