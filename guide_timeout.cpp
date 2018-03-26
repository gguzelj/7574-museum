#include <iostream>
#include "include/msg.h"
#include "include/shm.h"
#include "include/semaforo.h"
#include "include/logger.h"
#include "mensaje.h"
#include <time.h>

int main(int argc, char *argv[]) {
    init_logger(argv[0], getpid()); 
    int shm = getshm(7574);
    int *memoria = (int *) map(shm);
    // Semáforo para acceder a la memoria compartida
    int sem = getsem(0);
    int semGuia = getsem(GUIDE_SEM_ID);

    /////int t0 = time(NULL);

    while (true) {
        sleep(GUIDE_INTERVAL);
        p(sem); {
            if (memoria[3] > 0) {
                safelog("Timeout, arrancado el tour\n");
                v(semGuia);
            }
        } v(sem);
    }
}
