#!/usr/bin/python3

import argparse
import threading
import enum
import can


KEY_ID = 0x28B
SONG_ID = 0x28C


g_song_state = "ARAM"
g_stop = False


def control_song(interface: str) -> None:
    global g_song_state, g_stop

    zero_xor = can.Message(arbitration_id=KEY_ID, is_extended_id=False, data=b"\x00FM")

    with can.Bus(
        channel=interface,
        bustype="socketcan",
        can_filters=[{"can_id": SONG_ID, "can_mask": 0xFF8, "extended": False}],
    ) as bus:
        while not g_stop:
            bus.recv()
            bus.send(zero_xor)
            bus.send(
                can.Message(
                    arbitration_id=SONG_ID,
                    is_extended_id=False,
                    data=g_song_state.encode(),
                )
            )


def input_song(interface: str) -> None:
    global g_song_state, g_stop

    try:
        with can.Bus(
            channel=interface,
            bustype="socketcan",
            can_filters=[{"can_id": SONG_ID, "can_mask": 0xFF8, "extended": False}],
        ) as bus:
            while True:
                try:
                    g_song_state = input("Enter song:   ")
                except Exception:
                    print("Invalid input")
    finally:
        g_stop = True


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("interface")
    args = parser.parse_args()

    try:
        threading.Thread(target=control_song, args=(args.interface,)).start()
        input_song(args.interface)
    except KeyboardInterrupt:
        pass


if __name__ == "__main__":
    main()
