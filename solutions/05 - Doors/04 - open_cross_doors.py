import argparse
import threading
import can

DOORS_ID = 0x1F4
BASE_DATA = b"\x00\x00\x00\x00\x00\x00\x00"

g_state = 0


def get_doors(state: int):
    return state & 1, state & 2, state & 4, state & 8


def open_cross(current_state: int) -> int:
    global g_state

    prev_doors = get_doors(g_state)
    curr_doors = get_doors(current_state)
    ret_val = 0

    if prev_doors[0] != curr_doors[0]:
        ret_val ^= 8 if curr_doors[3] == 0 else 0
    if prev_doors[1] != curr_doors[1]:
        ret_val ^= 4 if curr_doors[2] == 0 else 0
    if prev_doors[2] != curr_doors[2]:
        ret_val ^= 2 if curr_doors[1] == 0 else 0
    if prev_doors[3] != curr_doors[3]:
        ret_val ^= 1 if curr_doors[0] == 0 else 0

    g_state = current_state
    return ret_val % 16


def lock_doors(interface: str) -> None:
    global g_state

    lock = can.Message(
        arbitration_id=DOORS_ID, is_extended_id=False, data=b"\x00" * 7
    )

    with can.Bus(
            channel=interface,
            bustype="socketcan",
            can_filters=[{"can_id": DOORS_ID, "can_mask": 0xFFF, "extended": False}],
    ) as bus:
        while True:
            message = bus.recv()
            # print(message.data[0] & 1)
            # print(message.data[0] & 2)
            # print(message.data[0] & 4)
            # print(message.data[0] & 8)
            lock.data = open_cross(message.data[0]).to_bytes(1, 'big') + lock.data
            bus.send(lock)


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("interface")
    args = parser.parse_args()

    try:
        lock_doors(args.interface)
    except KeyboardInterrupt:
        pass


if __name__ == "__main__":
    main()
