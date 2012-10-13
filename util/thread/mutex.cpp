#include "mutex.h"
#include <iostream>

TMutex::TMutex() : mut() {
    if (pthread_mutex_init(&mut, 0) !=0) {
        throw EMutex("Can't initialize mutex");
    }
}

TMutex::~TMutex() {
    if (pthread_mutex_destroy(&mut) != 0) {
        throw EMutex("Can't destroy mutex");
    }
}

void TMutex::Lock() {
    if (pthread_mutex_lock(&mut) != 0) {
        throw EMutex("Can't lock mutex");
    }
}

void TMutex::Unlock() {
    if (pthread_mutex_unlock(&mut) != 0) {
        throw EMutex("Can't unlock mutex");
    }
}


