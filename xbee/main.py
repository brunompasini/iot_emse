import binascii
import time
import machine
from digi import ble



# Change these two variables to your device's address and address type.
# The address and address type can be discovered using ble.gap_scan().
REMOTE_ADDRESS = "24:62:AB:D2:80:16"
address_type = ble.ADDR_TYPE_PUBLIC

address = binascii.unhexlify(REMOTE_ADDRESS.replace(':', ''))



user_button = machine.Pin(machine.Pin.board.D4, machine.Pin.IN, machine.Pin.PULL_UP)

ble.active(True)
print("Attempting connection to:", REMOTE_ADDRESS)

with ble.gap_connect(address_type, address) as conn:
    print("Connected")

    print("Discovering services, characteristics, and descriptors...")

    count = 0

    while True:
        services = list(conn.gattc_services())

        for service in services:
            print("Service", service)

            characteristics = list(conn.gattc_characteristics(service))

            for characteristic in characteristics:
                print("\tCharacteristic", characteristic)
        
                if characteristic[2] & ble.PROP_READ:
                    print("\t VALUE:", conn.gattc_read_characteristic(characteristic))
                


                if characteristic[2] & ble.PROP_WRITE:
                    time.sleep(1)
                    print("button pressing window")
                    time.sleep(1)

                    if user_button.value() == 0:
                        count += 1
                        if count > 6:
                            count = 0 
                            # Apparement module % ne marche pas :(
                    
                    print("stop pressing")
                    time.sleep(.5)
                    print("count : ", count)


                    if count==1:
                        conn.gattc_write_characteristic(characteristic,b"1")
                        print("\t VALUE:", conn.gattc_read_characteristic(characteristic))
                    elif count==2:
                        conn.gattc_write_characteristic(characteristic,b"2")
                        print("\t VALUE:", conn.gattc_read_characteristic(characteristic))
                    elif count==3:
                        conn.gattc_write_characteristic(characteristic,b"3")
                        print("\t VALUE:", conn.gattc_read_characteristic(characteristic))
                    elif count==4:
                        conn.gattc_write_characteristic(characteristic,b"4")
                        print("\t VALUE:", conn.gattc_read_characteristic(characteristic))
                    elif count==5:
                        conn.gattc_write_characteristic(characteristic,b"5")
                        print("\t VALUE:", conn.gattc_read_characteristic(characteristic))
                    elif count==6:
                        conn.gattc_write_characteristic(characteristic,b"6")
                        print("\t VALUE:", conn.gattc_read_characteristic(characteristic))
                    else:
                        conn.gattc_write_characteristic(characteristic,b"0")
                        print("\t VALUE:", conn.gattc_read_characteristic(characteristic))

                    time.sleep(1)


                            

        print("Done")
        time.sleep(.5)

"""
#time.sleep(1)




import machine
import sys
import utime
import time

# Pin definitions
user_button = machine.Pin(machine.Pin.board.D4, machine.Pin.IN, machine.Pin.PULL_UP)
#led = machine.Pin(machine.Pin.board.D4, machine.Pin.OUT)

# Blink forever
while True:
    print("button released")

    if user_button.value() == 0:
        print("button pressed")
    
    time.sleep(0.25)

    # Turn LED on and then off
                # Perform a descriptor discovery.
                # Because it is not valid to do another gatt operation during a
                # discovery (service, characteristic, or descriptor),
                # we store the descriptors into a list so that the descriptor
                # discovery process can complete before reading the attribute
                # values of the descriptors.
                descriptors = list(conn.gattc_descriptors(characteristic))
                print(descriptors)
                for descriptor in descriptors:
                    
                        


                    print("\t\tDescriptor", descriptor)
                    print("\t\t VALUE:", conn.gattc_read_descriptor(descriptor))"""