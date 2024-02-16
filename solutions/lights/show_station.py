#!/usr/bin/python3

import argparse
import threading
import enum
import can
import time


LIGHTS_ID = 0x29A
TURN_OFF_LIGHTS_DATA = b'\x00\x00\x00\x00\x00\x00\x00\x00'
TURN_ON_LIGHTS_DATA = b'\x01\x00\x00\x00\x00\x00\x00\x00'
LIGHTS_VOLUME_ID = 0x29B
LOW_LIGHTS_DATA = b'\x01\x00\x00\x00\x00\x00\x00\x00'
MEDIUM_LIGHTS_DATA = b'\x02\x00\x00\x00\x00\x00\x00\x00'
HIGH_LIGHTS_DATA = b'\x03\x00\x00\x00\x00\x00\x00\x00'
RADIO_ID = 0x22B


class LightsState(enum.IntEnum):
    NO_LIGHTS = 0
    LOW_LIGHTS = 1,
    MEDIUM_LIGHTS = 2,
    HIGH_LIGHTS = 3


g_lights_state = LightsState.NO_LIGHTS
g_stop = False


def control_lights(interface: str) -> None:
    global g_lights_state, g_stop

    turn_on_lights = can.Message(arbitration_id=LIGHTS_ID, is_extended_id=False, data=TURN_ON_LIGHTS_DATA)
    turn_off_lights = can.Message(arbitration_id=LIGHTS_ID, is_extended_id=False, data=TURN_OFF_LIGHTS_DATA)
    low_lights = can.Message(arbitration_id=LIGHTS_VOLUME_ID, is_extended_id=False, data=LOW_LIGHTS_DATA)
    medium_lights = can.Message(arbitration_id=LIGHTS_VOLUME_ID, is_extended_id=False, data=MEDIUM_LIGHTS_DATA)
    high_lights = can.Message(arbitration_id=LIGHTS_VOLUME_ID, is_extended_id=False, data=HIGH_LIGHTS_DATA)

    with can.Bus(channel=interface, bustype='socketcan', can_filters=[{"can_id": LIGHTS_ID, "can_mask": 0Xffe, "extended": False}]) as bus:
        while not g_stop:
            message = bus.recv()
            
            if message.arbitration_id == LIGHTS_ID:
                if g_lights_state == LightsState.NO_LIGHTS:
                    bus.send(turn_off_lights)
                else:
                    bus.send(turn_on_lights)
            elif message.arbitration_id == LIGHTS_VOLUME_ID:
                if g_lights_state == LightsState.LOW_LIGHTS:
                    bus.send(low_lights)
                elif g_lights_state == LightsState.MEDIUM_LIGHTS:
                    bus.send(medium_lights)
                elif g_lights_state == LightsState.HIGH_LIGHTS:
                    bus.send(high_lights)

def show_station_lights(interface: str) -> None:
    global g_lights_state, g_stop

    turn_on_lights = can.Message(arbitration_id=LIGHTS_ID, is_extended_id=False, data=TURN_ON_LIGHTS_DATA)
    turn_off_lights = can.Message(arbitration_id=LIGHTS_ID, is_extended_id=False, data=TURN_OFF_LIGHTS_DATA)
    low_lights = can.Message(arbitration_id=LIGHTS_VOLUME_ID, is_extended_id=False, data=LOW_LIGHTS_DATA)
    medium_lights = can.Message(arbitration_id=LIGHTS_VOLUME_ID, is_extended_id=False, data=MEDIUM_LIGHTS_DATA)
    high_lights = can.Message(arbitration_id=LIGHTS_VOLUME_ID, is_extended_id=False, data=HIGH_LIGHTS_DATA)

    try:
        with can.Bus(channel=interface, bustype='socketcan', can_filters=[{"can_id": RADIO_ID, "can_mask": 0Xfff, "extended": False}]) as bus:
            radio_station = bus.recv().data[2]

            for b in bin(radio_station)[2:]:
                if b == '1':
                    g_lights_state = LightsState.HIGH_LIGHTS
                    bus.send(turn_on_lights)
                    bus.send(high_lights)
                elif b == '0':
                    g_lights_state = LightsState.LOW_LIGHTS
                    bus.send(turn_on_lights)
                    bus.send(low_lights)
                
                time.sleep(0.5)
                g_lights_state = LightsState.NO_LIGHTS
                bus.send(turn_off_lights)
                time.sleep(0.5)
    finally:
        g_stop = True
    

def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument('interface')
    args = parser.parse_args()

    try:
        threading.Thread(target=control_lights, args=(args.interface,)).start()
        show_station_lights(args.interface)
    except KeyboardInterrupt:
        pass


if __name__ == '__main__':
    main()
