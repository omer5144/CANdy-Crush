#!/bin/bash

./build/keyboard.out vcan0 &    # dashboard
./build/dashboard.out vcan0 &       # keyboard

wait
