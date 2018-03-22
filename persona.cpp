#include "include/msg.h"
#include "mensaje.h"
#include <time.h>


unsigned int getRand();


int main(int argc, char *argv[]) {
    srand(time(NULL));

    if (argc < 3)
        return -1;
    int receive_queue_id = atoi(argv[1]);
    int send_queue_id = atoi(argv[2]);

    // Creamos las colas para cada puerta
    int receive_queue = creamsg(receive_queue_id);
    int send_queue = creamsg(send_queue_id);

    Mensaje m;
    m.mtype = 1;
    m.person_id = getpid();
    m.enter = true;
    // Pido entrar
    enviarmsg(send_queue, &m, sizeof(m));
    // Espero respuesta
    recibirmsg(receive_queue, &m, sizeof(m), 1);

    if (m.accepted) {
        //Simulo el paseo en el museo
        sleep(getRand());

        //Pido salir del museo
        m.enter = false;
        enviarmsg(send_queue, &m, sizeof(m));
        recibirmsg(receive_queue, &m, sizeof(m), 1);
    }
    return 0;
}

unsigned int getRand() {
    return (unsigned int) rand() % 1000;
}
