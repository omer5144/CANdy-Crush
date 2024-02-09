Instrument Cluster Simulator for SocketCAN
------------------------------------------

Compiling
---------
You will need:
* SDL2
* SDL2_image
* SDL2_ttf
* can-utils

You can get can-utils from github or on Ubuntu you may run the following

```
  sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl-ttf-dev can-utils  
```

With dependencies installed, you may build the project:

```bash
  gcc controls/*.c -o build/controls.out -I/usr/include/SDL2 -Wall -Wextra -lSDL2 -lSDL2_image -lSDL2_ttf
  gcc icsim/*.c -o build/icsim.out -I/usr/include/SDL2 -Wall -Wextra -lSDL2 -lSDL2_image -lSDL2_ttf
```

A build.sh file has also been provided with this repo.

Testing on a virtual CAN interface
----------------------------------
You can run the following commands to setup a virtual can interface

```
  sudo modprobe can
  sudo modprobe vcan
  sudo ip link add dev vcan0 type vcan
  sudo ip link set up vcan0
```

If you type ifconfig vcan0 you should see a vcan0 interface. A setup_vcan.sh file has also been provided with this
repo.

Usage
-----
Default operations:

Start the Instrument Cluster (IC) simulator:

```
  ./icsim.out vcan0
```

Then startup the controls

```
  ./controls.out vcan0
```

The hard coded defaults should be in sync and the controls should control the IC.  
The controls app will generate corrosponding CAN packets based on the buttons you press.  
The IC Sim sniffs the CAN and looks for relevant CAN packets that would change the display.

Troubleshooting
---------------
* If you get an error about canplayer then you may not have can-utils properly installed and in your path.
* If the controller does not seem to be responding make sure the controls window is selected and active

## lib.o not linking
If lib.o doesn't link it's probably because it's the wrong arch for your platform.  To fix this you will
want to compile can-utils and copy the newly compiled lib.o to the icsim directory.  You can get can-utils
from: https://github.com/linux-can/can-utils

## read: Bad address
When running `./icsim vcan0` you end up getting a `read: Bad Address` message,
this is typically a result of needing to recompile with updated SDL libraries.
Make sure you have the recommended latest SDL2 libraries.  Some users have
reported fixing this problem by creating symlinks to the SDL.h files manually
or you could edit the Makefile and change the CFLAGS to point to wherever your
distro installs the SDL.h header, ie: /usr/include/x86_64-linux-gnu/SDL2

There was also a report that on Arch linux needed sdl2_gfx library.
