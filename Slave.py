#!/usr/bin/env python

import socket
import sys

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
    
    request = ''.join(chr(x) for x in [13, 0x4A6F7921])
    s.sendall(request)
    
    response = s.recv(4096)

    # Set self as slave node on ring and get ring ID
    master_gid = int(str(response[0]))
    magic_number = int(str(response[1]) + str(response[2]) + str(response[3]) + str(response[4]))
    thisRID = int(str(response[5]))
    nextSlaveIP = int(str(response[6]) + str(response[7]) + str(response[8]) + str(response[9]))
    nextSlave_pretty = str(response[6]) + "." + str(response[7]) + "." + str(response[8]) + "." + str(response[9])

    # Display the GID of master, own ring ID, and the IP address of next slave
    print("Master GID: ", master_gid)
    print("My Ring ID: ", thisRID)
    print("Next Slave: ", nextSlave_pretty)

    # TODO: Prompt user for a ring ID and message

    # TODO: Send message to node

    # TODO: Display any packet addressed to this node

    # TODO: Forward any other packet with TTL > 1
    

if __name__ == '__main__':
    main(sys.argv)

