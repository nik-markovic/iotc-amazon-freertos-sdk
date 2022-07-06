# iotc-amazon-freertos-sdk

### Build Instructions

- Clone the Amazon FreeRTOS repo.
- Clone this repo into the libraries directory of the Amazon FreeRTOS repo.
- Follow the instructions for your board to be able to run the demo. For example, if running the PC simulator, you will need to set configNETWORK_INTERFACE_TO_USE in vendors/pc/boards/FreeRTOSConfig.h, etc.
- If using CMake, modify libraries/CMakeLists.txt - append at the end of file.
```cmake
add_subdirectory(${AFR_MODULES_DIR}/iotc-amazon-freertos-sdk iotc-amazon-freertos-sdk)
```
- If not using CMake, ensure that all the files in libraries/iotc-amazon-freertos-sdk are 
added appropriately to your project as include/source files.   
- Append to the beginning of the file demos/include/aws_credential.h:
```c
#include "iotconnect_sync.h"
#define clientcredentialMQTT_BROKER_ENDPOINT get_iothub_host()
#define clientcredentialIOT_THING_NAME iotc_sync_get_client_id()
```
