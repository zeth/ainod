#ifndef AINOD_JSONRPC_H
#define AINOD_JSONRPC_H

const char *process_buffer(char *buf,
                           bool silentnote,
                           char *req_id_format,
                           bool req_req_id);

#define AINOD_INVALID_REQ_ID "The 'request id' was not of required type "\
  "(as specified in the Req-id-format setting)."

#define AINOD_RPC_SILENT_NOTIFICATION 666

#endif /* AINOD_JSONRPC_H */
