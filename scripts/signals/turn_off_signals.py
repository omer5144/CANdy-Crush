#!/usr/bin/python3

import argparse
import can


SIGNAL_ID = 0x188
NO_SIGNAL_DATA = b'\x00\x00\x00\x00\x00\x00\x00\x00'


def turn_off_signals(interface: str) -> None:
    no_signal = can.Message(arbitration_id=SIGNAL_ID, is_extended_id=False, data=NO_SIGNAL_DATA)

    with can.Bus(channel=interface, bustype='socketcan', can_filters=[{"can_id": SIGNAL_ID, "can_mask": 0Xfff, "extended": False}]) as bus:
        while True:
            bus.recv()
            bus.send(no_signal)

def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument('interface')
    args = parser.parse_args()

    try:
        turn_off_signals(args.interface)
    except KeyboardInterrupt:
        pass


if __name__ == '__main__':
    main()
