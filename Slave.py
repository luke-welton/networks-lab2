#!/usr/bin/env python

import socket
import sys

def main(argv):
    try:
        master_hostname = argv[1]
        master_port = argv[2]
    except:
        print("Invalid arguments."
        sys.exit()

    # Send Join Ring Request

    # Set self as slave node on ring and get ring ID

    # Display the GID of master, own ring ID, and the IP address of next slave

    # TODO: Prompt user for a ring ID and message

    # TODO: Send message to node

    # TODO: Display any packet addressed to this node

    # TODO: Forward any other packet with TTL > 1
    

if __name__ == '__main__':
    main(sys.argv)

