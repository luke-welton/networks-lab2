#!/usr/bin/env python

import socket
import sys

def byte_to_int(byte):
    integer = 0
    for b in byte:
        integer += ord(b)
    return int(integer)

def print_hex(msg):
    print("Message in hexadecimal:")
    for i in range(10):
        print(msg[i].encode('hex'))


def main(argv):
    try:
        master_hostname = argv[1]
        master_port = argv[2] # for us this should be 10075
    except:
        print("Invalid arguments.")
        sys.exit()

    # Send Join Ring Request
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((master_hostname, int(master_port)))
    
    request = ''.join(chr(x) for x in [13, 0x4A, 0x6F, 0x79, 0x21])
    s.sendall(request) 

    response = s.recv(4096)

    #    print_hex(response)
    # Set self as slave node on ring and get ring ID
    master_gid = byte_to_int(response[0])
    # magic_number = int(str(response[1]) + str(response[2]) + str(response[3]) + str(response[4]))
    thisRID = byte_to_int(response[5])
    # nextSlaveIP = int(str(response[6]) + str(response[7]) + str(response[8]) + str(response[9]))
    nextSlave_pretty = str(byte_to_int(response[6])) + "." + str(byte_to_int(response[7])) + "." + str(byte_to_int(response[8])) + "." + str(byte_to_int(response[9]))

    # Display the GID of master, own ring ID, and the IP address of next slave
    print("Master GID: {}".format(master_gid))
    print("My Ring ID: {}".format(thisRID))
    print("Next Slave: {}".format(nextSlave_pretty))

    # TODO: Prompt user for a ring ID and message

    # TODO: Send message to node

    # TODO: Display any packet addressed to this node

    # TODO: Forward any other packet with TTL > 1
    

if __name__ == '__main__':
    main(sys.argv)

