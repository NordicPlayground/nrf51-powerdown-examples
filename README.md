nrf51-powerdown-examples
========================

This repository contains 2 example projects showing System-On and System-Off power-down modes and how to wake up from them. The naming and code of the projects should be fairly straight-forward. 

Requirements
------------
- nRF5 SDK version 11.0.0
- nRF51-DK or nRF52-DK development kits 
- nrfjprog or nrfgo Studio (for programming nRF5x)

The project may need modifications to work with other boards. 

To compile it, clone the repository into the \nRF5_SDK_11.0.0_89a8197\examples\peripheral\ folder.

Operation
---------
- Connect your nRF5x-DK to the PC via USB
- Erase nRF5x flash memory with command  nrfjprog --family NRF51 --eraseall  or  nrfjprog --family NRF52 --eraseall
- Compile the project you want to program
- Program the compiled nRF5x .hex file with command  nrfjprog --family NRF51 --program \_build\nrf51422_xxac.hex  or  nrfjprog --family NRF52 --program \_build\nrf52832_xxaa.hex
- Disconnect USB from the nRF5x-DK and power the board from a battery instead (for more accurate current consumption)
- Press Button 1 to enter System On low power mode
- Press Button 2 to exit System On low power mode

Note: If the nRF5x-DK board becomes unresponsive, you can recover the board with   nrfjprog --family NRF51 --recover  or  nrfjprog --family NRF52 --recover

About this project
------------------
This application is one of several applications that has been built by the support team at Nordic Semiconductor, as a demo of some particular feature or use case. It has not necessarily been thoroughly tested, so there might be unknown issues. It is hence provided as-is, without any warranty. 

However, in the hope that it still may be useful also for others than the ones we initially wrote it for, we've chosen to distribute it here on GitHub. 

The application is built to be used with the official nRF5 SDK, that can be downloaded from http://developer.nordicsemi.com/nRF5_SDK/.

Please post any questions about this project on https://devzone.nordicsemi.com.