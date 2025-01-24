ARAMobile Simulator for SocketCAN
------------------------------------------

Compiling
---------
You will need:
* SDL2
* SDL2_image
* SDL2_ttf
* can-utils

```
  sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl-ttf-dev can-utils  
```

With dependencies installed, you may build the project:

```bash
  gcc keyboard/*.c -o build/keyboard.out -I/usr/include/SDL2 -Wall -Wextra -lSDL2 -lSDL2_image -lSDL2_ttf
  gcc dashboard/*.c -o build/dashboard.out -I/usr/include/SDL2 -Wall -Wextra -lSDL2 -lSDL2_image -lSDL2_ttf
```

A build.sh file has also been provided with this repo.

Testing on a virtual CAN interface
----------------------------------
You can run the following commands to setup a virtual can interface

```bash
  sudo modprobe can
  sudo modprobe vcan
  sudo ip link add dev vcan0 type vcan
  sudo ip link set up vcan0
```

If you type ifconfig vcan0 you should see a vcan0 interface.  
A setup_vcan.sh file has also been provided with this repo.

Usage
-----
Default operations:

Start the keyboard:

```
  ./keyboard.out vcan0
```

Then startup the dashboard

```
  ./dashboard.out vcan0
```

The keyboard app will generate corrosponding CAN packets based on the buttons you press.  
The dashboard sniffs the CAN and looks for relevant CAN packets that would change the display.  
A run.sh file has also been provided with this repo.

Exercises
---------
The simulator designed for research use  
I order to use write python scripts that communicate with the can interface you will need the python-can library:

```bash
  pip install python-can
```

List of exercises ideas:

### Lock Doors
- Find the relevant ID and understand the value protocol
- Write a script that locks the doors forever
- Write a script that locks the oposite door when the driver tries to lock a door

### Lights
  - Find the relevant IDs and understand the value protocol
  - Write a script that disables the lights system
  - Write a script that switches the lights volume
    - low lights if the driver turns on the high lights
    - high lights if the driver turns on the low lights 
  - Write a script that fully controls the lights system - let the user choose to:
    1. turn off lights
    2. turn on low lights
    3. turn on medium lights
    4. turn on high lights

### Signals
- Find the relevant ID and understand the value protocol
- Write a script that disables the signals system
- Write a script that switches the signals direction
  - right signal if the driver turns left
  - left signal if the driver turns right
- Write a script that fully controls the signals system - let the user choose to:
  1. turn off signals
  2. turn on left signal
  3. turn on right signal
  4. turn on both signals

### Speed
- Write a script that locks the speed as the current speed
- Write a script that accelerates the speed
- Write a script that controls the speed and returns the control to the driver at the end- let the user choose to: 
  1. lock the speed
  2. accelerate
  3. decelerate

### Radio
  - Write a script that leaks the radio station number using the lights volume

A solutions directory with solutions has also been provided with this repo.  
The binary_search.sh script is used to find a specific command using the binary search algorithm

Images
------

### Dashboard

![](./data/dashboard_tutorial.png)

### Keyboard

![](./data/keyboard_tutorial.png)