#include <iostream>
#include "include/msg.h"
#include "include/logger.h"
#include "mensaje.h"

unsigned int getRand();

int main(int argc, char *argv[]) {
    char proc_name[100];
    sprintf(proc_name, "%s%d", argv[0], getpid());
    init_logger(proc_name, getpid());
    //log_exit();

    if (argc < 3)
        return -1;
    int receive_queue_id = atoi(argv[1]);
    int send_queue_id = atoi(argv[2]);

    // Accedemos a las colas para cada puerta
    int receive_queue = getmsg(receive_queue_id);
    int send_queue = getmsg(send_queue_id);

    long id = (long) getpid();
    Mensaje m(id);
    m.mtype = 1;
    m.enter = true;
    m.accepted = true;
    // Pido entrar
    safelog("Pido entrar");
    enviarmsg(send_queue, &m, sizeof(m));
    // Espero respuesta
    safelog("Espero respuesta");
    recibirmsg(receive_queue, &m, sizeof(m), 1);

    if (m.accepted) {
        // No debería pasar pero lo chequeo
        if (m.person_id != getpid()) {
            safeperror(m.person_id + " != " + getpid());
        }
        safelog("Fui aceptado. Entro y paseo");

        //Simulo el paseo en el museo
        sleep(getRand());

        //Pido salir del museo
        safelog("Pido salir");
        m.enter = false;
        enviarmsg(send_queue, &m, sizeof(m));
        safelog("Espero respuesta para salir");
        recibirmsg(receive_queue, &m, sizeof(m), 1);
    } else {
        safelog("Fui rechazado");
    }

    safelog("Me voy a casa");
    return 0;
}

unsigned int getRand() {
    return (unsigned int) rand() % 1000;
}
