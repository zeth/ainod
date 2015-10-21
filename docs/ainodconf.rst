ainod.conf
==========

NAME
----

ainod.conf - JSON daemon configuration file

SYNOPSIS
--------

/etc/ainod/ainod.conf

DESCRIPTION
-----------

This file configures various parameters of the JSON daemon

OPTIONS
-------

All options are configured in the "[Daemon]" section:

Workers
~~~~~~~

Takes a positive integer. Determines the number of worker child
processes to create on daemon start up. If not specified, it will be
set to the number of processor cores which is a nice conservative
minimum.

Increasing this number will increase capacity and performance but
being set too high can cause "thundering herd" type symptoms where the
increased processing cost of concurrency outweighs any benefit.

Twice the number of processor cores is a reasonable setting but the
best approach is to test your application and system for the ideal
number. In God we trust; all others must bring data.

Datadir
~~~~~~~

The location for JSON data files to be stored. If not specified, the
default is /var/lib/ainod

Require-protocol-version
~~~~~~~~~~~~~~~~~~~~~~~~

Takes a boolean argument of true or false. Defaults to false. If true,
the client's request will be rejected if it does not contain
{"jsonrpc": "2.0"}

Require-request-id
~~~~~~~~~~~~~~~~~~

Takes a boolean argument of true or false. Defaults to false. If true,
the client's request will be rejected if it does not contain an id
field identifying the request. Incidentally, this was the behaviour
outlined in JSON-RPC 1.0 specification.

Silent-notifications
~~~~~~~~~~~~~~~~~~~~

Takes a boolean argument of true or false. Defaults to false.

    Note: if Require-request-id (see above) is set to true, a missing
    request id will always result in an error and no further
    processing; obviously making this setting irrelevant in that
    case. The remaining text below assumes Require-request-id is set
    to false (the default).

If this setting is false (the default) then a missing request id or a
request id set to *null* will result in no special behaviour. The
daemon will carry on and return a successful result or error as
normal.

If this setting is true then a missing request id or a request id
set to *null* will result in the request being processed but no
response at all being returned, even if there was an error in the
request and the processing failed.

The JSON-RPC specifications name such a request as a 'notification'.
However, it is confusing for new users to send a request and receive
no response; especially when they have simply forgotten the id (or
sent null by mistake or design). Furthermore, use cases where the user
never cares about errors seem quite obscure. In the words of Tim
Peters:

Errors should never pass silently.
Unless explicitly silenced.

Remote-schemas
~~~~~~~~~~~~~~

Takes a boolean argument of true or false. Whether to fetch JSON
schema files from remote servers. Defaults to false. true is not
currently implemented.

EOF-marker
~~~~~~~~~~

Takes a boolean argument of true or false. Whether the data ends with
an End of File marker. If true then the daemon will reject any
incoming data without the EOF marker; this way it rejects incomplete
data earlier but reduces compatibility with some third party clients.

Req-id-format
~~~~~~~~~~~~~

Determines the format of the request id (if present). Available options are:

* default - allows anything in the request id
* rpc - allows strings, ints, numbers and null but not arrays or objects
* int - only allows int ids
* string - only allows string ids

If you have a real use case for another format, let us know.

.. _path-format:

Path-format
~~~~~~~~~~~

Affects how data is organised and stored.

* 0 - Traditional layout: /store/schema/id/
* 1 - Open collection layout: /store/collection/id/
* 2 - Collection of mixed schema layout: /store/id/schema/

Warning: Changing the Path-format while there is data in Ainod
requires writing your own script for moving the data into the new
format.
