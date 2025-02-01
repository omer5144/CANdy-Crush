#!/usr/bin/python3

import argparse
import can


LIGHTS_ID = 0x29A
TURN_OFF_LIGHTS_DATA = b"\x00\x00\x00\x00\x00\x00\x00\x00"


def disable_lights(interface: str) -> None:
    turn_off_lights = can.Message(
        arbitration_id=LIGHTS_ID, is_extended_id=False, data=TURN_OFF_LIGHTS_DATA
    )

    with can.Bus(
        channel=interface,
        bustype="socketcan",
        can_filters=[{"can_id": LIGHTS_ID, "can_mask": 0xFFF, "extended": False}],
    ) as bus:
        while True:
            bus.recv()
            bus.send(turn_off_lights)


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("interface")
    args = parser.parse_args()

    try:
        disable_lights(args.interface)
    except KeyboardInterrupt:
        pass


if __name__ == "__main__":
    main()
