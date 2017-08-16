#!/usr/bin/python3

"""Send an invalid request with missing method member."""

from simpleclient import send_data_to_socket

EXAMPLE = {
    "params": {
        "filter": {
            'store': 'catalog',
            'schema': 'product',
            'id': '704e418e-682d-4ade-99be-710f2208102e'
        }
    }
}


def main():
    """Send the example to the simple client."""
    send_data_to_socket(EXAMPLE)


if __name__ == '__main__':
    main()
