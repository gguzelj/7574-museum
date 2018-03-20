#include <iostream>
#include "./include/msg.h"
#include "include/shm.h"
#include "include/semaforo.h"

int MAX_PEOPLE = 100;
int MAX_PUERTA = 3;

int main() {

    int cola_recep, cola_envia;

    //Shared memory
    int shm = creashm(0, sizeof(int) * 2);
    int *memoria = (int *) map(shm);
    memoria[0] = MAX_PEOPLE;
    memoria[1] = 0;

    /*
    //Crear sem
    int sem = creasem(0);
    inisem(sem, 1);

    for (int i = 0; i < MAX_PUERTA; i++) {
        cola_recep = 2 * i;
        cola_envia = 2 * i + 1;
        if (fork() == 0) {
            std::cout << "Hello: " << getpid() << std::endl;
            execl("./puerta", "./puerta", cola_recep, cola_envia, 0);
            exit(0);
        }
    }
*/
    std::cout << "Hello, World!" << std::endl;
    return 0;
}