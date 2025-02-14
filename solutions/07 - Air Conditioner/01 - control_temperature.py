#!/usr/bin/python3

import argparse
import threading
import enum
import can
import struct


TEMPERATURE_ID = 0x3E7


def checksum(temperature: bytes) -> int:
    result = 0
    for b in temperature:
        while b:
            result += b & 1
            b >>= 1
    return result


def input_temperature(interface: str) -> None:
    try:
        with can.Bus(
            channel=interface,
            bustype="socketcan",
            can_filters=[
                {"can_id": TEMPERATURE_ID, "can_mask": 0xFFF, "extended": False}
            ],
        ) as bus:
            while True:
                try:
                    temperature = struct.pack("f", int(input("Enter temperature:   ")))

                    bus.send(
                        can.Message(
                            arbitration_id=TEMPERATURE_ID,
                            is_extended_id=False,
                            data=checksum(temperature).to_bytes(1, "big") + temperature,
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
        input_temperature(args.interface)
    except KeyboardInterrupt:
        pass


if __name__ == "__main__":
    main()
