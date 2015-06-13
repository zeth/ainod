#!/usr/bin/python3

"""Client test."""
import socket
import sys

ABSTRACT_SOCKET_NAME = "ainod"

def send_data_to_socket(socket_name=ABSTRACT_SOCKET_NAME):
    """Send command line arg text to socket."""
    data = " ".join(sys.argv[1:])

    sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

    to_be_sent = len(data)
    sent = 0
    last_blob = 0
    try:
        # Connect to server and send data
        sock.connect("\x00%s" % socket_name)

        sock.sendall(bytes(data + "\n", "utf-8"))
        #while sent < to_be_sent:
        ##    last_blob = sock.send(bytes(data + "\n", "utf-8")[sent:])
        #    sent += last_blob

        # Receive data from the server and shut down
        received = str(sock.recv(1024), "utf-8")

    except BrokenPipeError:
        print("Arrgh")
        sys.exit()
    finally:
        sock.close()

    print("Sent:     {}".format(len(data) +1))
    print("Received: {}".format(received))

def main():
    """Run the main socket."""
    send_data_to_socket()

if __name__ == '__main__':
    main()
