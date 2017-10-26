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

#ifndef AINOD_JSONRPC_H
#define AINOD_JSONRPC_H

const char *process_buffer(char *buf,
                           bool silentnote,
                           char *req_id_format,
                           bool req_req_id,
                           char *datadir,
                           int path_format);

#define AINOD_RPC_SILENT_NOTIFICATION 666

#define AINOD_INVALID_REQ_ID "The 'request id' was not of required type "\
  "(as specified in the Req-id-format setting)."

#define AINOD_METHOD_MISSING "The JSON sent is not a valid Request object, "\
  "it is missing the required 'method' member."

#define AINOD_CREATE_MISSING_DOCUMENT "The create method requires a "\
  "'document' object inside 'params'."



#endif /* AINOD_JSONRPC_H */
