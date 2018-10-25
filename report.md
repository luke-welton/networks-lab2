Group 13: Luke Welton, Jesse Roach, Bryant Kelley

# Introduction
This project implements a ring of communication between a TCP Master and a UDP Slave.
The Master is written in C++, and the Slave is written in Python.

# Functionality
In this lab, a Master is created as a TCP Server, allowing multiple UDP Clients to join as Slaves in a ring formation. In the next lab, they will be able to pass along messages to one another, but currently, they merely exist.

# Compiling
To compile the Master, you type `g++ Master.cpp -o Master` into the command line of a Linux machine with G++.

As the Slave is written in Python, a scripting language, no compilation is necessary.

# Executing
To execute the now-compiled Master, you enter `./Master <port>`, substituting `<port>` with the chosen port number.
As we are Group 13, we have been using 10075, but many other port numbers will work.

To execute the Slave, you enter `python Slave.py <name> <port>`, substituting `<name>` with the hostname of the Master and `<port>` with the same port number used with the Master.

# Results
