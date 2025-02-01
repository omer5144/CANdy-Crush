#!/usr/bin/python3

import argparse
import can


HORN_ID = 0x65
BEEP_DATA = b"\x00\x00\x00\x00\x00\x00\x00\x01"


def beep_constantly(interface: str) -> None:
    beep = can.Message(arbitration_id=HORN_ID, is_extended_id=False, data=BEEP_DATA)

    with can.Bus(
        channel=interface,
        bustype="socketcan",
        can_filters=[{"can_id": HORN_ID, "can_mask": 0xFFF, "extended": False}],
    ) as bus:
        bus.send(beep)
        while True:
            bus.recv()
            bus.send(beep)


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("interface")
    args = parser.parse_args()

    try:
        beep_constantly(args.interface)
    except KeyboardInterrupt:
        pass


if __name__ == "__main__":
    main()
