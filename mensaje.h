//
// Created by German Guzelj on 19/3/18.
//

#ifndef UNTITLED_MENSAJE_H
#define UNTITLED_MENSAJE_H

#include <iostream>

int CANT_PERSONAS = 30;
int CANT_PUERTAS = 3;

int GUIDE_SEM_ID = 9;
int GUIDE_QUEUE_ID = 6;
int PERSONAS_POR_GUIA = 5;
int GUIDE_INTERVAL = 2; // Representa 30 minutos

struct Mensaje {
    long mtype;
    long person_id;
    bool enter;
    bool accepted;

    Mensaje(long id) {
        person_id = id;
        ///memset((void *) person_id, 0, sizeof(person_id));
    }
};

#endif //UNTITLED_MENSAJE_H
