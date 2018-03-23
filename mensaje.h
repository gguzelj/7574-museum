//
// Created by German Guzelj on 19/3/18.
//

#ifndef UNTITLED_MENSAJE_H
#define UNTITLED_MENSAJE_H

#include <iostream>

int MAX_PEOPLE = 100;
int MAX_PUERTA = 3;

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
