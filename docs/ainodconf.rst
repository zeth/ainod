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
set to the number of processor cores which is a conservative minimum.

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

{ "$ref": "person.json" }

Require-protocol-version
~~~~~~~~~~~~~~~~~~~~~~~~

Takes a boolean argument of true or false. Defaults to false. If true,
the request will be rejected if it does not contain {"jsonrpc": "2.0"}

Require-request-id
~~~~~~~~~~~~~~~~~~

Takes a boolean argument of true or false. Defaults to false. If true,
the request will be rejected if it does not contain an id field
identifying the request.

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

