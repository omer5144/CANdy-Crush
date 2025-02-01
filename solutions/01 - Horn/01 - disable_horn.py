#!/usr/bin/python3

import argparse
import can


HORN_ID = 0x65
NO_BEEP_DATA = b"\x00\x00\x00\x00\x00\x00\x00\x00"


def disable_horn(interface: str) -> None:
    no_beep = can.Message(
        arbitration_id=HORN_ID, is_extended_id=False, data=NO_BEEP_DATA
    )

    with can.Bus(
        channel=interface,
        bustype="socketcan",
        can_filters=[{"can_id": HORN_ID, "can_mask": 0xFFF, "extended": False}],
    ) as bus:
        while True:
            bus.recv()
            bus.send(no_beep)


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("interface")
    args = parser.parse_args()

    try:
        disable_horn(args.interface)
    except KeyboardInterrupt:
        pass


if __name__ == "__main__":
    main()
