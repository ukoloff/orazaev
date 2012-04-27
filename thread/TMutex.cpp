#include "TMutex.h"
#include <iostream>

TMutex::TMutex() : mut() {
    if (pthread_mutex_init(&mut, 0) !=0) {
        std::cerr << "Can't initializied mutex" << std::endl;
        throw EInit();
    }
}

TMutex::~TMutex() {
    if (pthread_mutex_destroy(&mut) != 0) {
        std::cerr << "Can't destroy mutex" << std::endl;
        throw EDestroy();
    }
}

void TMutex::Lock() {
    if (pthread_mutex_lock(&mut) != 0) {
        std::cerr << "Can't lock mutex" << std::endl;
        throw ELock();
    }
}

void TMutex::Unlock() {
    if (pthread_mutex_unlock(&mut) != 0) {
        std::cerr << "Can't unlock mutex" << std::endl;
        throw EUnlock();
    }
}


