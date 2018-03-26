#include <iostream>
#include "include/msg.h"
#include "include/shm.h"
#include "include/semaforo.h"
#include "include/logger.h"
#include "mensaje.h"
#include <time.h>

unsigned int getRand();

int main(int argc, char *argv[]) {
    init_logger(argv[0], getpid());

    if (argc < 3) {
        std::cerr << "¡Cantidad de argumentos incorrecta!" << std::endl;//
        safeperror("Cantidad de argumentos incorrecta.\n");
        return -1;
    }
    int respuesta_queue_id = atoi(argv[1]);
    int solicitud_queue_id = atoi(argv[2]);

    // Accedemos a la memoria compartida
    int shm = getshm(7574);
    int *memoria = (int *) map(shm); // [0]: cant_max, [1]: cant_actual
    // Semáforo para acceder a la memoria compartida
    int sem = getsem(0);

    // Creamos las colas para cada puerta
    int respuesta_queue = getmsg(respuesta_queue_id);
    int solicitud_queue = getmsg(solicitud_queue_id);

    safelog("PUERTA\n");

    while (true) {
        Mensaje m(0);
        recibirmsg(solicitud_queue,  &m, sizeof(m), 1);

        safelog("Recibí mensaje de %d\n", m.person_id);
        if (m.enter) {
            safelog("Es para entrar\n");
            // Simulo atender al cliente
            sleep(getRand());

            p(sem); {
                if (m.accepted = memoria[1] < memoria[0]) {
                    memoria[1]++;
                    safelog("Lo dejo entrar. Cantidad actual: %d\n", memoria[1]);
                } else {
                    safelog("NO lo dejo entrar. Cantidad actual: %d\n", memoria[1]);
                }
            } v(sem);
        } else {
            safelog("Es para salir\n");
            // Simulo atender al cliente
            sleep(getRand());
            m.accepted = true;

            p(sem); {
                if (memoria[1] < 0) {
                    safeperror("Cantidad negativa de visitantes\n");
                }
                memoria[1]--;
                safelog("Lo dejo salir. Cantidad actual: %d\n", memoria[1]);
            } v(sem);
        }

        enviarmsg(solicitud_queue, &m, sizeof(m));
    }
}

unsigned int getRand() {
    unsigned int siesta = rand() % 4;
    safelog("Siesta: %d\n", siesta);//
    return siesta;
}
