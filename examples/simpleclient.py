#!/usr/bin/python3

"""Simple Client."""
import socket
import sys
import json

ABSTRACT_SOCKET_NAME = "ainod"


def send_data_to_socket(data,
                        socket_name=ABSTRACT_SOCKET_NAME):
    """Send command line arg text to socket."""
    data = json.dumps(data)
    sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

    try:
        # Connect to server and send data
        sock.connect("\x00%s" % socket_name)

        sock.sendall(bytes(data + "\n", "utf-8"))

        # Receive data from the server and shut down
        received = str(sock.recv(4096), "utf-8")

    except BrokenPipeError:
        print("Arrgh")
        sys.exit()
    finally:
        sock.close()

    print("Sent:     {}".format(len(data) + 1))
    print("Received: {}".format(received))
