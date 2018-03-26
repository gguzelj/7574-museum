#include <iostream>
#include "include/msg.h"
#include "include/shm.h"
#include "include/semaforo.h"
#include "include/logger.h"
#include "mensaje.h"
#include <time.h>

int main(int argc, char *argv[]) {
    init_logger(argv[0], getpid());
    int cantGuiados = 0;
    // Accedemos a la memoria compartida
    int shm = getshm(7574);
    int *memoria = (int *) map(shm);
    // Semáforo para acceder a la memoria compartida
    int sem = getsem(0);
    int semGuia = getsem(GUIDE_SEM_ID);
    // Micrófono del guía
    int guide_queue = getmsg(GUIDE_QUEUE_ID);
    Mensaje m(0);

    // Correr el timeout process
    if (fork() == 0) {
        execl("./guide_timeout", "./guide_timeout", (char *) NULL);
        exit(0);
    }

    while (true) {
        // Esperar que le liberen el semáforo
        p(semGuia);
        safelog("Iniciando Tour\n");
        p(sem); {
            cantGuiados = memoria[3];
            memoria[3] = 0;
        } v(sem);

        if (cantGuiados > 0) {
            // Mandar PERSONAS_POR_GUIA mensajes
            for (int i = 0; i < cantGuiados; ++i) {
              safelog("Avisando a cada persona\n");
              enviarmsg(guide_queue, &m, sizeof(m));
            }
        }
        // Da el tour
    }
}
