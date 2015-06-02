#ifndef AINOD_WORKER_H
#define AINOD_WORKER_H


void child_worker(int worker,
                  pthread_mutex_t *mp,
                  char *datadir,
                  int incoming);

#endif /* AINOD_WORKER_H */
