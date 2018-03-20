#include <iostream>
#include "include/msg.h"
#include "include/shm.h"
#include "include/semaforo.h"
#include "mensaje.h"

unsigned int getRand();

int main(int argc, char *argv[]) {

    int receive_queue_id = atoi(argv[1]);
    int send_queue_id = atoi(argv[2]);

    //creamos la memoria compartida
    int shm = getshm(0);
    int *memoria = (int *) map(shm); //[0]: cant_max, [1]: cant_actual



    //creamos las colas para cada puerta
    int receive_queue = creamsg(receive_queue_id);
//    int send_queue = creamsg(send_queue_id);

    int sem = getsem(0);


    Mensaje m;
    m.mtype = 1;
    m.accepted = true;
    m.enter = true;
    m.person_id = 2;

    enviarmsg(receive_queue, &m, sizeof(m));




    Mensaje m2;
    m2.mtype=1;
    recibirmsg(receive_queue,  &m2, sizeof(m2), 1);
    std::cout << m2.person_id << " " << m2.enter << std::endl;



/*
    while (true) {
        Mensaje m;
        recibirmsg(receive_queue,  &m, sizeof(m), 1);

        if (m.enter) {
            //simulo atender al cliente
            sleep(getRand());

            p(sem);
            {
                m.accepted = memoria[1] <= memoria[0];
                if (m.accepted) {
                    memoria[1]++;
                }
            }
            v(sem);
        } else {
            //simulo atender al cliente
            sleep(getRand());
            m.accepted = true;
            p(sem);
            {
                memoria[1]--;
            }
            v(sem);
        }

        enviarmsg(send_queue, &m, sizeof(m));

    }
*/
}

unsigned int getRand() {
    return (unsigned int) rand() % 100;
}







