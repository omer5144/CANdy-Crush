#!/usr/bin/python3

import argparse
import can


LIGHTS_VOLUME_ID = 0x29B
LOW_LIGHTS = b"\x01\x00\x00\x00\x00\x00\x00\x00"
HIGH_LIGHTS = b"\x03\x00\x00\x00\x00\x00\x00\x00"


def switch_lights(interface: str) -> None:
    low_lights = can.Message(
        arbitration_id=LIGHTS_VOLUME_ID, is_extended_id=False, data=LOW_LIGHTS
    )
    high_lights = can.Message(
        arbitration_id=LIGHTS_VOLUME_ID, is_extended_id=False, data=HIGH_LIGHTS
    )

    with can.Bus(
        channel=interface,
        bustype="socketcan",
        can_filters=[
            {"can_id": LIGHTS_VOLUME_ID, "can_mask": 0xFFF, "extended": False}
        ],
    ) as bus:
        while True:
            message = bus.recv()
            if message.data == LOW_LIGHTS:
                bus.send(high_lights)
            elif message.data == HIGH_LIGHTS:
                bus.send(low_lights)


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("interface")
    args = parser.parse_args()

    try:
        switch_lights(args.interface)
    except KeyboardInterrupt:
        pass


if __name__ == "__main__":
    main()
