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


create
------

Creates a document or documents.

:Parameters: * *document*: The document(s) to insert
             * *filter*: A query that matches the document(s) to create
   

delete
------

Deletes a document or documents.

:Parameters: * *filter*: A query that matches the document(s) to delete

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
