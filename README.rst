Ainod - JSON Storage and Retrieval Daemon
=========================================

Ainod is a daemon (background service) that accepts JSON objects and
stores and retrieves them for you.

You could describe ainod as a NoSQL database or a document database
but ainod uses plain text JSON files as the primary storage format
rather than a proprietary binary data format.

**This is very early, many essential features do not exist yet.**

**Read on if you want to help develop ainod, otherwise come back later**

Documentation
-------------

To build the documentation you need the Sphinx documentation generator

On Debian, Ubuntu, Raspbian and similar .deb based distributions, you
can use this command::

    sudo apt-get install python3-sphinx

Then::

    cd docs
    make html
    make man

The resulting HTML documentation is in _build/html/ and the man page
documentation is in _build/man/

Depepencies
-----------

Ainod is written in C and targets operating systems that have the
Linux kernel such as Debian, Fedora, Raspbian etc.

To compile the binary, you need the development headers for json-c and
uuid. You also the headers for systemd on platforms that use that
(most modern systems except Gentoo).

On Debian, Ubuntu, Raspbian and similar .deb based distributions, you
can use this command::

    sudo apt-get install libjson-c-dev libsystemd-dev uuid-dev

You will also need a C build environment which you can install using::

    sudo apt-get install build-essential autoconf pkg-config

On Fedora or similar .rpm based distributions, you can use::

    dnf install json-c-devel systemd-devel uuid-devel

or if you are old school::

    yum install json-c-devel systemd-devel uuid-devel

Installation
------------

Compile ainod using::

    ./autogen.sh
    ./configure
    make

To install it to the system::

    sudo make install

If you don't want to install it systemwide, you can run ainod from
userspace using::

    cd src
    ./ainod

Unit Tests
----------

Unit test support is not built in by default. To run the unit tests,
you need to install check.

To install check, you can use the package manager again, for
example::

    sudo apt-get install check

Once you have check installed, you need to compile ainod with tests
enabled::

    ./autogen.sh --enable-tests
    make

The make check command runs the tests::

    make check
