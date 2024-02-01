#!/bin/bash

trap 'kill $(jobs -p)' EXIT

./build/controls.out vcan0 &         # dashboard
./build/icsim.out vcan0 &      # joystick

wait
