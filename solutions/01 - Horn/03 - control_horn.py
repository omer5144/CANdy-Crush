#!/usr/bin/python3

import argparse
import threading
import enum
import can


HORN_ID = 0x65
NO_BEEP_DATA = b"\x00\x00\x00\x00\x00\x00\x00\x00"
BEEP_DATA = b"\x00\x00\x00\x00\x00\x00\x00\x01"


class HornState(enum.IntEnum):
    NO_BEEP = 0
    BEEP = 1


g_horn_state = HornState.NO_BEEP
g_stop = False


def control_horn(interface: str) -> None:
    global g_horn_state, g_stop

    no_beep = can.Message(
        arbitration_id=HORN_ID, is_extended_id=False, data=NO_BEEP_DATA
    )
    beep = can.Message(arbitration_id=HORN_ID, is_extended_id=False, data=BEEP_DATA)

    with can.Bus(
        channel=interface,
        bustype="socketcan",
        can_filters=[{"can_id": HORN_ID, "can_mask": 0xFFF, "extended": False}],
    ) as bus:
        while not g_stop:
            bus.recv()
            if g_horn_state == HornState.NO_BEEP:
                bus.send(no_beep)
            elif g_horn_state == HornState.BEEP:
                bus.send(beep)


def input_horn() -> None:
    global g_horn_state, g_stop

    try:
        while True:
            try:
                g_horn_state = int(input("Enter horn state: (0 - none, 1 - beep):   "))
            except Exception:
                print("Invalid input")
    finally:
        g_stop = True


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("interface")
    args = parser.parse_args()

    try:
        threading.Thread(target=control_horn, args=(args.interface,)).start()
        input_horn()
    except KeyboardInterrupt:
        pass


if __name__ == "__main__":
    main()
