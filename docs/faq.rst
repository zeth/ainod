Frequently Asked Questions
==========================

What is ainod?
--------------

ainod is a minimalist daemon to help your application store data in an
organised set of JSON files.

Is ainod a database?
--------------------

Not really, well it depends on how you define a database!

If it helps you can think of ainod as a NoSQL database or a document
database that uses plain text JSON files as the primary storage format
rather than a proprietary binary data format.

If it helps you can think of ainod as a high-level data mapper or data
access layer that could go over the top of a database (but the
database got lost in the post).

However, it is really just a little daemon (or water spirit, see
https://en.wikipedia.org/wiki/Aino_%28mythology%29 ).

What operating systems does ainod support
-----------------------------------------

ainod should work on most modern Linux kernel-based distributions like
Debian Fedora or Raspbian.

The unit tests only run on Intel platforms currently.

What about Linux systems without systemd?
-----------------------------------------

When systemd is available, ainod uses it to start up as a daemon, for
logging to the journal, to support socket-based activation and for
management (i.e. turning it off/restarting it).

Gentoo uses OpenRC so we need to learn how to write an init file to
support that. Help from an experienced Gentoo packager would be most
welcome.

If all else fails, you can just run the ainod binary using ./ainod
(see below)

What are the dependencies of ainod?
-----------------------------------

To compile the binary, you need the development headers for json-c and
uuid. You also the headers for systemd on platforms that use that
(most modern systems except Gentoo - see above).

On Debian, Ubuntu, Raspbian and similar .deb based distributions, you
can use this command::

    sudo apt-get install libjson-c-dev libsystemd-dev uuid-dev

You will also need a C build environment which you can install using::

    sudo apt-get install build-essential autoconf

On Fedora or similar .rpm based distributions, you can use::

    dnf install json-c-devel systemd-devel uuid-devel

or if you are old school::

    yum install json-c-devel systemd-devel uuid-devel

How do I install ainod
----------------------

We hope one day to have pre-built ainod binaries, preferably available
from the distribution's own package management system. Until then, you
need to compile it.

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


How do I run the unit tests?
----------------------------

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


Why does ainod not support other operating systems
--------------------------------------------------

Currently ainod makes no attempt at supporting other operating
systems. Firstly, it is easier to get the overall design first
without adding until later all the 'if Linux do this, if Windows do
that' checks.

However, there is nothing really stopping us adding other operating
systems except time and available test hardware. Support for other
operating systems may emerge if there is a some corporate sponsorship
for us to add it or someone in the community wants to add it for us.
