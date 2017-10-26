/***
  This file is part of ainod.

  Copyright 2015 Zeth

  ainod is free software; you can redistribute it and/or modify it
  under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation; either version 2.1 of the License, or
  (at your option) any later version.

  ainod is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with ainod; If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef AINOD_JSON_SCHEMA_H
#define AINOD_JSON_SCHEMA_H

/** JSON RPC ERRORS **/

/** Invalid JSON was received by the server. **/
#define JSON_RPC_ERROR_PARSE_ERROR -32700

/** The JSON sent is not a valid Request object. */
#define JSON_RPC_ERROR_INVALID_REQUEST -32600

/** The method does not exist / is not available. **/
#define JSON_RPC_ERROR_METHOD_NOT_FOUND -32601

/** Invalid method parameter(s). */
#define JSON_RPC_ERROR_INVALID_PARAMS -32602

/** Internal JSON-RPC error. */
#define JSON_RPC_ERROR_INTERNAL_ERROR -32603

/** Reserved for implementation-defined server-errors. */
#define JSON_RPC_ERROR_SERVER_ERROR -32000


/** METHOD ERRORS **/

/** Create method has an error */
#define AINOD_METHOD_CREATE_ERROR -20000

#endif /* AINOD_JSON_SCHEMA_H */
