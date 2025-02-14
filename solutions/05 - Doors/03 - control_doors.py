#!/usr/bin/python3

import argparse
import threading
import enum
import can


DOORS_ID = 0x1F4
BASE_DATA = b"\x00\x00\x00\x00\x00\x00\x00"


g_doors_state = 0
g_stop = False


def control_doors(interface: str) -> None:
    global g_doors_state, g_stop

    with can.Bus(
        channel=interface,
        bustype="socketcan",
        can_filters=[{"can_id": DOORS_ID, "can_mask": 0xFFF, "extended": False}],
    ) as bus:
        while not g_stop:
            bus.recv()
            bus.send(
                can.Message(
                    arbitration_id=DOORS_ID,
                    is_extended_id=False,
                    data=g_doors_state.to_bytes(1, "big") + BASE_DATA,
                )
            )


def input_doors(interface: str) -> None:
    global g_doors_state, g_stop

    g_doors_state = 0
    try:
        with can.Bus(
            channel=interface,
            bustype="socketcan",
            can_filters=[{"can_id": DOORS_ID, "can_mask": 0xFFF, "extended": False}],
        ) as bus:
            while True:
                try:
                    bus.send(
                        can.Message(
                            arbitration_id=DOORS_ID,
                            is_extended_id=False,
                            data=g_doors_state.to_bytes(1, "big") + BASE_DATA,
                        )
                    )

                    g_doors_state ^= (
                        1
                        << int(
                            input(
                                "Enter and toggle doors state: (0 - left front, 1 - right front, 2 - left back, 0 - right back):   "
                            )
                        )
                    ) % 16

                except Exception:
                    print("Invalid input")
    finally:
        g_stop = True


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("interface")
    args = parser.parse_args()

    try:
        threading.Thread(target=control_doors, args=(args.interface,)).start()
        input_doors(args.interface)
    except KeyboardInterrupt:
        pass


if __name__ == "__main__":
    main()
