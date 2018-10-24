Group 13: Luke Welton, Jesse Roach, Bryant Kelley

# Introduction
This project implements a ring of communication between a TCP Master and a UDP Slave.
The Master is written in C++.
The Slave is written in Python.

# Functionality

# Compiling
To compile the Master, you type `g++ Master.cpp -o Master` into a Linux machine with G++.
As the Slave is written in Python, a scripting language, no compilation is necessary.

# Executing
To execute the now-compiled Master, you enter `./Master <port>`, substituting `<port>` with the chosen port number.
As we are Group 13, we have been using 10023, but many other port numbers will work.

To execute the Slave, you enter `python Slave.py Master <port>`, substituting `<port>` with the same port number used with the Master.

# Results
