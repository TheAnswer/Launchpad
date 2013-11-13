#ifndef SINGLEINSTANCE_H
#define SINGLEINSTANCE_H

#include "Windows.h"

class SingleInstance {
protected:
    HANDLE instanceMutex;
    DWORD lastError;
public:
    SingleInstance(TCHAR* mutexName) {
        instanceMutex = CreateMutex(NULL, FALSE, mutexName);
        lastError = GetLastError();
    }

    ~SingleInstance() {
        if (instanceMutex) {
            CloseHandle(instanceMutex);
            instanceMutex = NULL;
        }
    }

    bool isAnotherInstanceRunning() {
        return (ERROR_ALREADY_EXISTS == lastError);
    }

};

#endif // SINGLEINSTANCE_H
