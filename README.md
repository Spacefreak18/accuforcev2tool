# AccuforceV2Tool
Tool providing basic configuration for the Accuforce V2 Direct Drive Sim Racing Wheel

## Features
- Recalibrate Wheel Center

## Dependencies
- libhidapi-hidraw , uses hidapi so this can potentially be cross platform by using the appropriate hidapi backend for each os

## Usage
```
./accuforcev2tool -d /dev/hidraw1 center
```
- the wheel should show two devices in dmesg. Specify the hidrawX device that corresponds with the device that is NOT input. It should not say input,hidrawX.

## ToDo
 - Support setting different intensity levels, range, etc
 - much, much more

