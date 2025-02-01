#!/usr/bin/python3

import argparse
import can


SPEED_ID = 0x3C4
DELTA = 10


def change_speed(speed_data: bytes, delta: int) -> bytes:
    return (
        b"\x00\x00\x00"
        + max(
            min((int.from_bytes(speed_data[3:5], "big") + delta), 0xFFFF), 0
        ).to_bytes(2, "big")
        + b"\x00\x00\x00"
    )


def accelerate(interface: str) -> None:
    with can.Bus(
        channel=interface,
        bustype="socketcan",
        can_filters=[{"can_id": SPEED_ID, "can_mask": 0xFFF, "extended": False}],
    ) as bus:
        speed_data = bus.recv().data

        while True:
            bus.recv()

            for _ in range(2):
                speed_data = change_speed(speed_data, DELTA)
                bus.send(
                    can.Message(
                        arbitration_id=SPEED_ID, is_extended_id=False, data=speed_data
                    )
                )


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("interface")
    args = parser.parse_args()

    try:
        accelerate(args.interface)
    except KeyboardInterrupt:
        pass


if __name__ == "__main__":
    main()
