#ifndef __zdev_thread_hpp__
#define __zdev_thread_hpp__

#include "../sys/zbase.hpp"
#include <windows.h>

ZDEV_NAMESPACE_BEGIN

class Thread {
private:
    HANDLE th;
    static DWORD WINAPI thread_function(void *self) { ((Thread *)self)->run();  return 0; }
public:
    /** �߳������� */
    virtual void run() = 0;
    
    /** �����߳� */
    void start() {
        th = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread_function, this, 0, NULL);
    }
    
    /** ǿ���˳��߳� */
    void kill() {
        ::TerminateThread(th, 0);
    }
    
    /** �ȴ��߳̽��� */
    void join() {
        ::WaitForSingleObject(th, INFINITE);
    }
    
    /** ���� */
    static void sleep(unsigned long milliseconds) {
        ::Sleep(milliseconds);
    }

    /** ���ص�ǰ�߳� ID */
    static ulong getCurrentId() {
        return (ulong) ::GetCurrentThreadId();
    }
    
    /** ���� cpu ʱ��Ƭ */
    static void yield() {
        ::Sleep(0);        
    }
    
};



ZDEV_NAMESPACE_END

#endif //__zdev_thread_hpp__
