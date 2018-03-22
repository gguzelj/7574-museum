#include <iostream>
#include "include/msg.h"
#include "include/shm.h"
#include "include/semaforo.h"
#include "mensaje.h"

unsigned int getRand();

int main(int argc, char *argv[]) {

    if (argc < 3)
        return -1;
    int receive_queue_id = atoi(argv[1]);
    int send_queue_id = atoi(argv[2]);
    std::cout << 5 << std::endl;

    // Creamos la memoria compartida
    ///int shm = getshm(7574);
    ///int *memoria = (int *) map(shm); //[0]: cant_max, [1]: cant_actual
    // Semáforo para acceder a la memoria compartida.
    int sem = getsem(0);
    std::cout << 4 << std::endl;

    // Creamos las colas para cada puerta
    int receive_queue = creamsg(receive_queue_id);
    std::cout << 3 << std::endl;//
//    int send_queue = creamsg(send_queue_id);

    ///Testing
    Mensaje m;
    std::cout << 0 << std::endl;
    m.mtype = 1;
    m.enter = true;
    m.person_id = 2;
    m.accepted = true;
    std::cout << 1 << std::endl;//

    enviarmsg(receive_queue, &m, sizeof(m));
    std::cout << 2 << std::endl;//




    Mensaje m2;
    m2.mtype = 1;
    recibirmsg(receive_queue,  &m2, sizeof(m2), 1);
    std::cout << m2.person_id << " " << m2.enter << std::endl;
    ///


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
