#include "include/msg.h"
#include "mensaje.h"


unsigned int getRand();


int main(int argc, char *argv[]) {

    int receive_queue_id = atoi(argv[1]);
    int send_queue_id = atoi(argv[2]);

    //creamos las colas para cada puerta
    int receive_queue = creamsg(receive_queue_id);
    int send_queue = creamsg(send_queue_id);

    Mensaje m;
    m.person_id = getpid();
    m.enter = true;
    enviarmsg(send_queue, &m, sizeof(m));
    recibirmsg(receive_queue, &m, sizeof(m), 1);

    if (m.accepted) {
        //Simulo el paseo en el museo
        sleep(getRand());

        //Trato de salir del museo
        m.enter = false;
        enviarmsg(send_queue, &m, sizeof(m));
        recibirmsg(receive_queue, &m, sizeof(m), 1);
    }

}

unsigned int getRand() {
    return (unsigned int) rand() % 1000;
}







