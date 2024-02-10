#!/bin/bash

# Function to handle interrupt signal
interrupt_handler() {
    echo "run.sh interrupted. Stopping simulator."
    kill -TERM $pid1 $pid2
    exit 1
}

# Register interrupt handler
trap 'interrupt_handler' INT

# Start the first program in the background
./build/keyboard.out vcan0 &
pid1=$!

# Start the second program in the background
./build/dashboard.out vcan0 &
pid2=$!

# Wait for both programs to complete
wait $pid1
status1=$?

wait $pid2
status2=$?

# Check the exit statuses
if [ $status1 -eq 0 ] && [ $status2 -eq 0 ]; then
    echo "Simulator completed successfully."
else
    echo "Error: One or both simulator parts failed."
fi
