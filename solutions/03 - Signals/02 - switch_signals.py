#!/usr/bin/python3

import argparse
import can


SIGNAL_ID = 0x188
LEFT_SIGNAL_DATA = b"\x01\x00\x00\x00\x00\x00\x00\x00"
RIGHT_SIGNAL_DATA = b"\x02\x00\x00\x00\x00\x00\x00\x00"


def switch_signals(interface: str) -> None:
    left_signal = can.Message(
        arbitration_id=SIGNAL_ID, is_extended_id=False, data=LEFT_SIGNAL_DATA
    )
    right_signal = can.Message(
        arbitration_id=SIGNAL_ID, is_extended_id=False, data=RIGHT_SIGNAL_DATA
    )

    with can.Bus(
        channel=interface,
        bustype="socketcan",
        can_filters=[{"can_id": SIGNAL_ID, "can_mask": 0xFFF, "extended": False}],
    ) as bus:
        while True:
            message = bus.recv()
            if message.data == LEFT_SIGNAL_DATA:
                bus.send(right_signal)
            elif message.data == RIGHT_SIGNAL_DATA:
                bus.send(left_signal)


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("interface")
    args = parser.parse_args()

    try:
        switch_signals(args.interface)
    except KeyboardInterrupt:
        pass


if __name__ == "__main__":
    main()
