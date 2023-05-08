# Ultra-Low Power in System On Mode

If the Seeed Studio XIAO nRF52840 (xiao_ble) does not get configured correctly, System On sleep will consume ~1mA of power. 
The Problem is caused by the SPI Flash P25Q16H. It apparently does not get put to sleep automatically.
Additionally there are other small changes, that reduce the power to single digit uA.

# Building and Running
To build the sample with Visual Studio Code, follow the steps listed on the [How to build an application](https://nrfconnect.github.io/vscode-nrf-connect/get_started/build_app_ncs.html) 
page in the nRF Connect for VS Code extension documentation. 
See [Building and programming an application](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/getting_started/programming.html#gs-programming) for other building and programming scenarios 
and [Testing and debugging an application](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/getting_started/testing.html#gs-testing) for general information about testing and debugging in the nRF Connect SDK.

# Flashing
Flashing is done via SWD. 
This demo is not set up to be flashed via the default xiao_ble bootloader.
To possibly do this, CONFIG_BUILD_OUTPUT_UF2=y, CONFIG_USE_DT_CODE_PARTITION=y and CONFIG_USB_DEVICE_STACK=y should be set in prj.conf
This will, at least slightly, increase power consumption and is untested by me.
