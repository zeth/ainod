Filter
======

Filter is a JSON object specifying elements which must be present for
a document to be included in the result set.

It the main way of querying the data available through ainod.

Using a full reference
~~~~~~~~~~~~~~~~~~~~~~

If a *ref* parameter is provided, then it overrides any other
parameters given to the get method, and those redundant parameters do
not need to be provided.

:Parameters: * *ref*: the full reference of the target document

Querying a store
~~~~~~~~~~~~~~~~

If *ref* is not provided, then *store* has to be. 

:Parameters: * *store*: the store to get a document from
             * *schema*: the schema used by the target document
             * *id* (optional): the id of the target document

Using open collection format
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If ainod has been configured to use the non-default open collection
:ref:`Path Format <path-format>`, then instead of specifying a
*schema*, a *collection* must be specified.

:Parameters: * *store*: the store to get a document from
             * *collection*: the collection to get a document from
             * *id* (optional): the id of the target document
