#!/usr/bin/python3

import argparse
import threading
import enum
import can


SPEED_ID = 0x244
DELTA = 10


class SpeedState(enum.IntEnum):
    LOCK_SPEED = 0
    ACCELERATE = 1
    DECELERATE = 2


g_speed_state = SpeedState.LOCK_SPEED
g_stop = False


def compare_speed(speed_data1: bytes, speed_data2: bytes) -> int:
    return int.from_bytes(speed_data1[3:5], "big") - int.from_bytes(
        speed_data2[3:5], "big"
    )


def change_speed(speed_data: bytes, delta: int) -> bytes:
    return (
        b"\x00\x00\x00"
        + max(
            min((int.from_bytes(speed_data[3:5], "big") + delta), 0xFFFF), 0
        ).to_bytes(2, "big")
        + b"\x00\x00\x00"
    )


def control_speed(interface: str) -> None:
    global g_signal_state, g_stop

    with can.Bus(
        channel=interface,
        bustype="socketcan",
        can_filters=[{"can_id": SPEED_ID, "can_mask": 0xFFF, "extended": False}],
    ) as bus:
        speed_data = bus.recv().data

        while not g_stop:
            bus.recv()

            for _ in range(2):
                if g_speed_state == SpeedState.LOCK_SPEED:
                    pass
                elif g_speed_state == SpeedState.ACCELERATE:
                    speed_data = change_speed(speed_data, DELTA)
                elif g_speed_state == SpeedState.DECELERATE:
                    speed_data = change_speed(speed_data, -DELTA)

                bus.send(
                    can.Message(
                        arbitration_id=SPEED_ID, is_extended_id=False, data=speed_data
                    )
                )

        stop = False
        while not stop:
            real_speed_data = bus.recv().data

            for _ in range(2):
                if compare_speed(real_speed_data, speed_data) >= DELTA // 2:
                    speed_data = change_speed(speed_data, DELTA)
                elif compare_speed(speed_data, real_speed_data) >= DELTA // 2:
                    speed_data = change_speed(speed_data, -DELTA)
                else:
                    stop = True

                bus.send(
                    can.Message(
                        arbitration_id=SPEED_ID, is_extended_id=False, data=speed_data
                    )
                )


def input_speed() -> None:
    global g_speed_state, g_stop

    try:
        while True:
            try:
                g_speed_state = int(
                    input(
                        "Enter speed state: (0 - lock, 1 - accelerate, 2 - decelerate):   "
                    )
                )
            except Exception:
                print("Invalid input")
    finally:
        g_stop = True


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("interface")
    args = parser.parse_args()

    try:
        threading.Thread(target=control_speed, args=(args.interface,)).start()
        input_speed()
    except KeyboardInterrupt:
        pass


if __name__ == "__main__":
    main()
