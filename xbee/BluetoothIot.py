#!/usr/bin/env python
# coding: utf-8

# In[ ]:



import binascii
from digi import ble

# Change these two variables to your device's address and address type.
# The address and address type can be discovered using ble.gap_scan().
REMOTE_ADDRESS = "24:62:AB:D2:92:9A"
address_type = ble.ADDR_TYPE_PUBLIC

# Put address into bytes (without colons)
address = binascii.unhexlify(REMOTE_ADDRESS.replace(':', ''))

ble.active(True)
print("Attempting connection to:", REMOTE_ADDRESS)

# Context manager will automatically close() the connection upon completion
with ble.gap_connect(address_type, address) as conn:
    print("Connected")

    print("Discovering services, characteristics, and descriptors...")

    # It is not valid to do another gatt operation during another discovery
    # (service, characteristic, or descriptor discovery).
    # Do the service discovery first, storing the services into a list
    # and then do the characteristic discovery.
    services = list(conn.gattc_services())

    # Print out the services and characteristics
    for service in services:
        print("Service", service)

        # Perform a characteristic discovery.
        # Because it is not valid to do another gaatt operation during a
        # discovery (service, characteristic, or descriptor),
        # we store the characteristics into a list so that the characteristic
        # discovery process can complete before discovering descriptors or
        # reading the attribute values of the characteristics.
        characteristics = list(conn.gattc_characteristics(service))

        for characteristic in characteristics:
            print("\tCharacteristic", characteristic)
            # Check the properties of the discovered characteristic
            if characteristic[2] & ble.PROP_READ:
                # If the characteristic attribute is readable, read and print it
                print("\t VALUE:", conn.gattc_read_characteristic(characteristic))

            # Perform a descriptor discovery.
            # Because it is not valid to do another gatt operation during a
            # discovery (service, characteristic, or descriptor),
            # we store the descriptors into a list so that the descriptor
            # discovery process can complete before reading the attribute
            # values of the descriptors.
            descriptors = list(conn.gattc_descriptors(characteristic))
            for descriptor in descriptors:
                print("\t\tDescriptor", descriptor)
                print("\t\t VALUE:", conn.gattc_read_descriptor(descriptor))

print("Done")

