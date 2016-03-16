Ainod - JSON Storage and Retrieval Daemon
=========================================

Ainod is a daemon (background service) that accepts JSON objects and
stores and retrieves them for you.

You could describe ainod as a NoSQL database or a document database
but ainod uses plain text JSON files as the primary storage format
rather than a proprietary binary data format.

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

    sudo apt-get install build-essential autoconf

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
you need both Valgrind and the NovaProva unit test
framework. Instructions for getting the latter are here:
http://novaprova.readthedocs.org/en/latest/getting-started.html

Sadly NovaProva does not yet support ARM or any other non-Intel
architecture so you cannot run the tests on Raspbian. Hopefully, that
gets fixed someday at least for Raspbian. Ainod does however work on
Raspbian, just the test runner doesn't work.

To install Valgrind, you can use the package manager again, for
example::

    sudo apt-get install valgrind

Once you have both NovaProva and Valgrind installed, you need to
compile ainod with tests enabled::

    ./autogen.sh --enable-tests
    make

The make check command runs the tests::

    make check