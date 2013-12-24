#include "windebugmonitor.h"

#include "atlbase.h"
#include "atlstr.h"

WinDebugMonitor::WinDebugMonitor() {
    m_hDBWinMutex = NULL;
    m_hDBMonBuffer = NULL;
    m_hEventBufferReady = NULL;
    m_hEventDataReady = NULL;

    m_hWinDebugMonitorThread = NULL;
    m_bWinDebugMonStopped = TRUE;
    m_pDBBuffer = NULL;
}

WinDebugMonitor::~WinDebugMonitor() {
    Unintialize();
}

DWORD WinDebugMonitor::Initialize()
{
    DWORD errorCode = 0;
    BOOL bSuccessful = FALSE;

    SetLastError(0);

    // Mutex: DBWin
    // ---------------------------------------------------------
    CComBSTR DBWinMutex = L"DBWinMutex";
    m_hDBWinMutex = ::OpenMutex(
        MUTEX_ALL_ACCESS,
        FALSE,
        DBWinMutex
        );

    if (m_hDBWinMutex == NULL) {
        errorCode = GetLastError();

        emit outputDebugString(-1, "Failed to create DBWinMutex");
        return errorCode;
    }

    // Event: buffer ready
    // ---------------------------------------------------------
    CComBSTR DBWIN_BUFFER_READY = L"DBWIN_BUFFER_READY";
    m_hEventBufferReady = ::OpenEvent(
        EVENT_ALL_ACCESS,
        FALSE,
        DBWIN_BUFFER_READY
        );

    if (m_hEventBufferReady == NULL) {
        m_hEventBufferReady = ::CreateEvent(
            NULL,
            FALSE,    // auto-reset
            TRUE,    // initial state: signaled
            DBWIN_BUFFER_READY
            );

        if (m_hEventBufferReady == NULL) {
            errorCode = GetLastError();

            emit outputDebugString(-1, "Failed to create m_hEventBufferReady");
            return errorCode;
        }
    }

    // Event: data ready
    // ---------------------------------------------------------
    CComBSTR DBWIN_DATA_READY = L"DBWIN_DATA_READY";
    m_hEventDataReady = ::OpenEvent(
        SYNCHRONIZE,
        FALSE,
        DBWIN_DATA_READY
        );

    if (m_hEventDataReady == NULL) {
        m_hEventDataReady = ::CreateEvent(
            NULL,
            FALSE,    // auto-reset
            FALSE,    // initial state: nonsignaled
            DBWIN_DATA_READY
            );

        if (m_hEventDataReady == NULL) {
            errorCode = GetLastError();

            emit outputDebugString(-1, "Failed to create m_hEventDataReady");
            return errorCode;
        }
    }

    // Shared memory
    // ---------------------------------------------------------
    CComBSTR DBWIN_BUFFER = L"DBWIN_BUFFER";
    m_hDBMonBuffer = ::OpenFileMapping(
        FILE_MAP_READ,
        FALSE,
        DBWIN_BUFFER
        );

    if (m_hDBMonBuffer == NULL) {
        m_hDBMonBuffer = ::CreateFileMapping(
            INVALID_HANDLE_VALUE,
            NULL,
            PAGE_READWRITE,
            0,
            sizeof(struct dbwin_buffer),
            DBWIN_BUFFER
            );

        if (m_hDBMonBuffer == NULL) {
            errorCode = GetLastError();

            emit outputDebugString(-1, "Failed to create m_hDBMonBuffer");
            return errorCode;
        }
    }

    m_pDBBuffer = (struct dbwin_buffer *)::MapViewOfFile(
        m_hDBMonBuffer,
        SECTION_MAP_READ,
        0,
        0,
        0
        );

    if (m_pDBBuffer == NULL) {
        errorCode = GetLastError();

        emit outputDebugString(-1, "Failed to create m_pDBBuffer");
        return errorCode;
    }

    // Monitoring thread
    // ---------------------------------------------------------
    m_bWinDebugMonStopped = FALSE;

    m_hWinDebugMonitorThread = ::CreateThread(
        NULL,
        0,
        WinDebugMonitorThread,
        this,
        0,
        NULL
        );

    if (m_hWinDebugMonitorThread == NULL) {
        m_bWinDebugMonStopped = TRUE;
        errorCode = GetLastError();

        emit outputDebugString(-1, "Failed to create m_hWinDebugMonitorThread");
        return errorCode;
    }

    // set monitor thread's priority to highest
    // ---------------------------------------------------------
    bSuccessful = ::SetPriorityClass(
        ::GetCurrentProcess(),
        REALTIME_PRIORITY_CLASS
        );

    bSuccessful = ::SetThreadPriority(
        m_hWinDebugMonitorThread,
        THREAD_PRIORITY_TIME_CRITICAL
        );

    //::WaitForSingleObjec(m_hWinDebugMonitorThread, INFINITE);

    return errorCode;
}

void WinDebugMonitor::Unintialize()
{
    if (m_hWinDebugMonitorThread != NULL) {
        m_bWinDebugMonStopped = TRUE;
        ::WaitForSingleObject(m_hWinDebugMonitorThread, INFINITE);
    }

    if (m_hDBWinMutex != NULL) {
        CloseHandle(m_hDBWinMutex);
        m_hDBWinMutex = NULL;
    }

    if (m_hDBMonBuffer != NULL) {
        ::UnmapViewOfFile(m_pDBBuffer);
        CloseHandle(m_hDBMonBuffer);
        m_hDBMonBuffer = NULL;
    }

    if (m_hEventBufferReady != NULL) {
        CloseHandle(m_hEventBufferReady);
        m_hEventBufferReady = NULL;
    }

    if (m_hEventDataReady != NULL) {
        CloseHandle(m_hEventDataReady);
        m_hEventDataReady = NULL;
    }

    m_pDBBuffer = NULL;
}

DWORD WINAPI WinDebugMonitor::WinDebugMonitorThread(void *pData)
{
    WinDebugMonitor *_this = (WinDebugMonitor *)pData;

    if (_this != NULL) {
        while (!_this->m_bWinDebugMonStopped) {
            _this->WinDebugMonitorProcess();
        }
    }

    return 0;
}

DWORD WinDebugMonitor::WinDebugMonitorProcess()
{
    DWORD ret = 0;

    // wait for data ready
    ret = ::WaitForSingleObject(m_hEventDataReady, TIMEOUT_WIN_DEBUG);

    if (ret == WAIT_OBJECT_0) {
        //m_pDBBuffer->dwProcessI
        OutputWinDebugString(m_pDBBuffer->dwProcessId, m_pDBBuffer->data);

        // signal buffer ready
        SetEvent(m_hEventBufferReady);
    }

    return ret;
}
