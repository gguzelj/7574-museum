//
// Created by German Guzelj on 19/3/18.
//

#ifndef UNTITLED_MENSAJE_H
#define UNTITLED_MENSAJE_H

#include <iostream>

struct Mensaje {
    long mtype;
    long person_id;
    bool enter;
    bool accepted;

    Mensaje() {
        memset((void *) person_id, 0, sizeof(person_id));
    }
};

#endif //UNTITLED_MENSAJE_H
