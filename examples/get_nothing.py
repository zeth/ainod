#!/usr/bin/python3

"""Very simple get example."""

from simpleclient import send_data_to_socket

EXAMPLE = {
    "method": "get",
    "params": {
        "filter": {
            'store': 'catalog',
            'schema': 'product',
            'id': 'Something-that-does-not-exist'
        }
    }
}


def main():
    """Send the example to the simple client."""
    send_data_to_socket(EXAMPLE)

if __name__ == '__main__':
    main()
