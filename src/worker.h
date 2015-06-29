#ifndef AINOD_WORKER_H
#define AINOD_WORKER_H

#define BACKLOG 10
#define MAX_EPOLL_FD 4096
#define MAXEVENTS 64
#define PAGE_SIZE 4096

void child_worker(int worker,
                  pthread_mutex_t *mp,
                  char *datadir,
                  int incoming,
                  bool silentnote,
                  char *req_id_format,
                  bool req_req_id,
                  int path_format);

#endif /* AINOD_WORKER_H */
