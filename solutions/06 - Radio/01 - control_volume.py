#!/usr/bin/python3

import argparse
import threading
import enum
import can


VOLUME_ID = 0x28A


def input_volume(interface: str) -> None:
    try:
        with can.Bus(
            channel=interface,
            bustype="socketcan",
            can_filters=[{"can_id": VOLUME_ID, "can_mask": 0xFFF, "extended": False}],
        ) as bus:
            while True:
                try:
                    volume = int(input("Enter volume difference:   "))

                    bus.send(
                        can.Message(
                            arbitration_id=VOLUME_ID,
                            is_extended_id=False,
                            data=volume.to_bytes(4, "little", signed=True),
                        )
                    )

                except Exception:
                    print("Invalid input")
    finally:
        pass


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("interface")
    args = parser.parse_args()

    try:
        input_volume(args.interface)
    except KeyboardInterrupt:
        pass


if __name__ == "__main__":
    main()
