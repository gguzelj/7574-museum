#include <iostream>
#include "include/msg.h"
#include "include/shm.h"
#include "include/semaforo.h"
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

    // Creamos la memoria compartida
    int shm = getshm(7574);
    int *memoria = (int *) map(shm); //[0]: cant_max, [1]: cant_actual
    // Semáforo para acceder a la memoria compartida
    int sem = getsem(0);
    int semGen = getsem(1);

    // Creamos las colas para cada puerta
    int receive_queue = creamsg(receive_queue_id);
    int send_queue = creamsg(send_queue_id);

    p(sem); {
        if (memoria[2]++ == MAX_PUERTA) {
            v(semGen);
        }
    } v(sem);

    while (true) {
        Mensaje m(0);
        recibirmsg(receive_queue,  &m, sizeof(m), 1);

        safelog("Recibí mensaje de " + m.person_id);
        if (m.enter) {
            safelog("Es para entrar");
            // Simulo atender al cliente
            sleep(getRand());

            p(sem); {
                if (m.accepted = memoria[1] <= memoria[0]) {
                    memoria[1]++;
                    safelog("Lo dejo entrar. Cantidad actual: " + memoria[1]);
                }
            } v(sem);
        } else {
            safelog("Es para salir");
            // Simulo atender al cliente
            sleep(getRand());
            m.accepted = true;

            p(sem); {
                if (memoria[1] < 0) {
                    safeperror("Cantidad negativa de visitantes");
                }
                memoria[1]--;
                safelog("Lo dejo salir. Cantidad actual: " + memoria[1]);
            } v(sem);
        }

        enviarmsg(send_queue, &m, sizeof(m));
    }
}

unsigned int getRand() {
    return (unsigned int) rand() % 100;
}
