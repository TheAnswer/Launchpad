#ifndef WINDEBUGMONITOR_H
#define WINDEBUGMONITOR_H

#include "Windows.h"

#include <iostream>

#include <QObject>

//http://www.codeproject.com/Articles/23776/Mechanism-of-OutputDebugString

class WinDebugMonitor : public QObject {
Q_OBJECT
private:
    enum {
        TIMEOUT_WIN_DEBUG    =    100
    };

    struct dbwin_buffer
    {
        DWORD   dwProcessId;
        char    data[4096-sizeof(DWORD)];
    };

private:
    HANDLE m_hDBWinMutex;
    HANDLE m_hDBMonBuffer;
    HANDLE m_hEventBufferReady;
    HANDLE m_hEventDataReady;

    HANDLE m_hWinDebugMonitorThread;
    BOOL m_bWinDebugMonStopped;
    struct dbwin_buffer *m_pDBBuffer;

private:
    void Unintialize();
    DWORD WinDebugMonitorProcess();
    static DWORD WINAPI WinDebugMonitorThread(void *pData);

signals:
    void outputDebugString(int, QString);

public:
    WinDebugMonitor();
    ~WinDebugMonitor();

    DWORD Initialize();
    BOOL IsInitialized() {
        return !m_bWinDebugMonStopped;
    }

public:
    virtual void OutputWinDebugString(int processId, const char *str) {
        emit outputDebugString(processId, QString(str));
    }
};

#endif // WINDEBUGMONITOR_H
