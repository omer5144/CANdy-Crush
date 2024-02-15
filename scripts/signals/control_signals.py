#!/usr/bin/python3

import argparse
import threading
import enum
import can


SIGNALS_ID = 0x188
NO_SIGNALS_DATA = b'\x00\x00\x00\x00\x00\x00\x00\x00'
LEFT_SIGNAL_DATA = b'\x01\x00\x00\x00\x00\x00\x00\x00'
RIGHT_SIGNAL_DATA = b'\x02\x00\x00\x00\x00\x00\x00\x00'
BOTH_SIGNALS_DATA = b'\x03\x00\x00\x00\x00\x00\x00\x00'


class SignalsState(enum.IntEnum):
    NO_SIGNALS = 0
    LEFT_SIGNAL = 1,
    RIGHT_SIGNAL = 2,
    BOTH_SIGNALS = 3


g_signals_state = SignalsState.NO_SIGNALS
g_stop = False


def control_signals(interface: str) -> None:
    global g_signals_state, g_stop

    no_signals = can.Message(arbitration_id=SIGNALS_ID, is_extended_id=False, data=NO_SIGNALS_DATA)
    left_signal = can.Message(arbitration_id=SIGNALS_ID, is_extended_id=False, data=LEFT_SIGNAL_DATA)
    right_signal = can.Message(arbitration_id=SIGNALS_ID, is_extended_id=False, data=RIGHT_SIGNAL_DATA)
    both_signals = can.Message(arbitration_id=SIGNALS_ID, is_extended_id=False, data=BOTH_SIGNALS_DATA)

    with can.Bus(channel=interface, bustype='socketcan', can_filters=[{"can_id": SIGNALS_ID, "can_mask": 0Xfff, "extended": False}]) as bus:
        while not g_stop:
            bus.recv()
            bus.send(no_signals)

            bus.recv()
            if g_signals_state == SignalsState.NO_SIGNALS:
                bus.send(no_signals)
            elif g_signals_state == SignalsState.LEFT_SIGNAL:
                bus.send(left_signal)
            elif g_signals_state == SignalsState.RIGHT_SIGNAL:
                bus.send(right_signal)
            elif g_signals_state == SignalsState.BOTH_SIGNALS:
                bus.send(both_signals)

def input_signals() -> None:
    global g_signals_state, g_stop

    try:
        while True:
            try:
                g_signals_state = int(input("Enter signals state: (0 - none, 1 - left, 2 - right, 3 - both):   "))
            except Exception:
                print('Invalid input')
    finally:
        g_stop = True
    

def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument('interface')
    args = parser.parse_args()

    try:
        threading.Thread(target=control_signals, args=(args.interface,)).start()
        input_signals()
    except KeyboardInterrupt:
        pass


if __name__ == '__main__':
    main()
