#include <iostream>
#include "include/msg.h"
#include "include/shm.h"
#include "include/semaforo.h"
#include "include/logger.h"
#include "mensaje.h"
#include <time.h>

unsigned int getRand();

void visitaGuiada();

int main(int argc, char *argv[]) {
    init_logger(argv[0], getpid());
    log_exit();

    if (argc < 3) {
        safeperror("Persona con menos argumentos que los necesarios.\n");
        return -1;
    }
    int respuesta_queue_id = atoi(argv[1]);
    int solicitud_queue_id = atoi(argv[2]);

    // Accedemos a las colas para cada puerta
    int respuesta_queue = getmsg(respuesta_queue_id);
    int solicitud_queue = getmsg(solicitud_queue_id);

    long id = (long) getpid();
    Mensaje m(id);
    m.mtype = 1;
    m.enter = true;
    m.accepted = true;
    // Pido entrar
    safelog("Pido entrar\n");
    enviarmsg(solicitud_queue, &m, sizeof(m));
    // Espero respuesta
    safelog("Espero respuesta\n");
    recibirmsg(respuesta_queue, &m, sizeof(m), 1);

    if (m.accepted) {
        // No debería pasar pero lo chequeo
        if (m.person_id != getpid()) {
            safeperror(m.person_id + " != " + getpid());
        }
        safelog("Fui aceptado. Entro y paseo\n");

        //Simulo el paseo en el museo
        if (rand() % 3 == 0) {
          visitaGuiada();
        } else {
          sleep(getRand());
        }

        //Pido salir del museo
        safelog("Pido salir\n");
        m.enter = false;
        enviarmsg(solicitud_queue, &m, sizeof(m));
        safelog("Espero respuesta para salir\n");
        recibirmsg(respuesta_queue, &m, sizeof(m), 1);
    } else {
        safelog("Fui rechazado! :((((\n");
    }

    safelog("Me voy a casa\n");
    return 0;
}

unsigned int getRand() {
    return (unsigned int) rand() % 30;
}

void visitaGuiada() {
    // Accedemos a la memoria compartida
    int shm = getshm(7574);
    int *memoria = (int *) map(shm);
    // Semáforo para acceder a la memoria compartida
    int sem = getsem(0);
    int semGuia = getsem(GUIDE_SEM_ID);
    int guide_queue = getmsg(GUIDE_QUEUE_ID);
    Mensaje m(0);
    safelog("Espero a que salga el tour\n");

    p(sem); {
        if (++memoria[3] == PERSONAS_POR_GUIA) {
            v(semGuia);
        }
    } v(sem);

    recibirmsg(guide_queue, &m, sizeof(m), 1);

    // Paseamos felices
    sleep(getRand() * 3);
}
