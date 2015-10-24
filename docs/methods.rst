Methods
=======

This document explains the built-in RPC methods of ainod.

Ideally higher level drivers in everyone's favourite programming
language or framework would be created and shared by the community,
then every application does not have to re-implement clients for these
methods.

get
---

Gets a document or documents. 

:Parameters: * *filter*: A query that matches the document(s) to get

save
----

Saves a document or document. Acts the same as the most relevant
method (e.g. create or replace).

Useful when you don't want to be bothered too much.

:Parameters: * *document*: The document(s) to insert
             * *filter*: A query that matches the document(s) to save


create
------

Creates a document or documents.

:Parameters: * *document*: The document(s) to insert
             * *filter*: A query that matches the document(s) to create

If the document already exits, DuplicateKeyError.

delete
------

Deletes a document or documents.

:Parameters: * *filter*: A query that matches the document(s) to delete

Returns the number (quantity) of deleted documents.

If the document already exists then no error but returns 0.

update
------

Updates a document or documents.

:Parameters: * *filter*: A query that matches the document(s) to update
             * *update*: The modifications to apply.
             * *upsert* (optional): If true, perform an insert if no documents match the filter.

replace
-------

Replaces a document or documents.

:Parameters: * *filter*: A query that matches the document(s) to replace.
             * *replacement*: The new document(s).
             * *upsert* (optional): If true, perform an insert if no documents match the filter.

reindex
-------

Reindexes a document or documents.

:Parameters: * *filter*: A query that matches the document(s) to index/reindex

count
-----

Counts a number of documents that match a filter

:Parameters: * *filter*: A query that matches the document(s) to count
