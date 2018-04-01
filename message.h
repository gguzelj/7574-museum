#ifndef UNTITLED_MENSAJE_H
#define UNTITLED_MENSAJE_H

#include <iostream>

struct Message {
    long mtype;
    long person_id;
    bool enter;
    bool accepted;
};

Message* Message_new(long mtype) {
    Message* response = static_cast<Message *>(malloc(sizeof(Message)));
    memset(response, 0, sizeof(Message));
    response->mtype = mtype;
    return response;
}

Message* Message_new(long mtype, long person_id, bool enter, bool accepted) {
    Message* response = static_cast<Message *>(malloc(sizeof(Message)));
    response->mtype = mtype;
    response->person_id = person_id;
    response->enter = enter;
    response->accepted = accepted;
    return response;
}

#endif //UNTITLED_MENSAJE_H
