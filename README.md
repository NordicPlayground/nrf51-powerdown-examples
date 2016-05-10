nrf51-powerdown-examples
========================

This repository contains 2 example projects showing System-On and System-Off power-down modes and how to wake up from them. The naming and code of the projects should be fairly straight-forward. 

Requirements
------------
- [nRF5 SDK version 11.0.0](http://developer.nordicsemi.com/nRF5_SDK/nRF5_SDK_v11.x.x/) 
- [nRF51-DK](https://www.nordicsemi.com/eng/Products/nRF51-DK) or [nRF52-DK](https://www.nordicsemi.com/eng/Products/Bluetooth-Smart-Bluetooth-low-energy/nRF52-DK) development kits 
- nrfjprog or nrfgo Studio (for programming nRF5x)

The project may need modifications to work with other boards. 

To compile it, clone the repository into the *\nRF5_SDK_11.0.0_89a8197\examples\peripheral\* folder.

Operation
---------
- Connect your nRF5x-DK to the PC via USB
- Erase nRF5x flash memory with command  **nrfjprog --family NRF51 --eraseall**  or  **nrfjprog --family NRF52 --eraseall**
- Compile the project you want to program
- Program the compiled nRF5x .hex file with command  **nrfjprog --family NRF51 --program \_build\nrf51422_xxac.hex**  or  **nrfjprog --family NRF52 --program \_build\nrf52832_xxaa.hex**
- Disconnect USB from the nRF5x-DK and power the board from a battery instead (for more accurate current consumption)
- Press Button 1 to enter System On low power mode
- Press Button 2 to exit System On low power mode

Note: If the nRF5x-DK board becomes unresponsive, you can recover the board with   **nrfjprog --family NRF51 --recover**  or  **nrfjprog --family NRF52 --recover**

Expected current consumption
----------------------------
The table below shows expected current consumption for nRF51 and nRF52 low power modes with different RAM retention. These numbers can be derived from the power chapter in the [nRF51822](http://infocenter.nordicsemi.com/topic/com.nordic.infocenter.nrf51/dita/nrf51/pdflinks/51822_ps.html?cp=2_3_0) / [nRF51422](http://infocenter.nordicsemi.com/topic/com.nordic.infocenter.nrf51/dita/nrf51/pdflinks/51422_ps.html?cp=2_4_0) / [nRF52832](http://infocenter.nordicsemi.com/topic/com.nordic.infocenter.nrf52832.ps.v1.0/power.html?cp=1_3_0_16_9_0#unique_289196923) Product Specification.

Low power mode | nRF51(uA) | nRF52(uA)
--------- | --------- | ---------
System Off - no RAM retention | 0.6 | 0.7
System Off - 16kB RAM retention | 1.8 | 0.8
System Off - 32kB RAM retention | 3.0 | 0.9
System Off - 64kB RAM retention | - | 1.0
System On - no RAM retention | 1.4 | 1.2
System On - 16kB RAM retention | 2.6 | 1.3
System On - 32kB RAM retention | 3.8 | 1.4
System On - 64kB RAM retention | - | 1.5

About this project
------------------
This application is one of several applications that has been built by the support team at Nordic Semiconductor, as a demo of some particular feature or use case. It has not necessarily been thoroughly tested, so there might be unknown issues. It is hence provided as-is, without any warranty. 

However, in the hope that it still may be useful also for others than the ones we initially wrote it for, we've chosen to distribute it here on GitHub. 

The application is built to be used with the official nRF5 SDK, that can be downloaded from [http://developer.nordicsemi.com/nRF5_SDK/](http://developer.nordicsemi.com/nRF5_SDK/)

Please post any questions about this project on [https://devzone.nordicsemi.com](https://www.nordicsemi.com/)
