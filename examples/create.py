#!/usr/bin/python3

"""Very simple create example."""

from simpleclient import send_data_to_socket

EXAMPLE = {
    "method": "create",
    "params": {
        'filter': {
            'store': 'catalog',
            'schema': 'product',
        },
        'document': {
            "name": "A yellow window",
            "price": 200.75,
            "tags": ["home", "yellow"]
        }
    }
}


def main():
    """Send the example to the simple client."""
    send_data_to_socket(EXAMPLE)

if __name__ == '__main__':
    main()
