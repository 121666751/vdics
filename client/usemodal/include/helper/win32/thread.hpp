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
    /** 线程运行体 */
    virtual void run() = 0;
    
    /** 启动线程 */
    void start() {
        th = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread_function, this, 0, NULL);
    }
    
    /** 强制退出线程 */
    void kill() {
        ::TerminateThread(th, 0);
    }
    
    /** 等待线程结束 */
    void join() {
        ::WaitForSingleObject(th, INFINITE);
    }
    
    /** 休眠 */
    static void sleep(unsigned long milliseconds) {
        ::Sleep(milliseconds);
    }

    /** 返回当前线程 ID */
    static ulong getCurrentId() {
        return (ulong) ::GetCurrentThreadId();
    }
    
    /** 放弃 cpu 时间片 */
    static void yield() {
        ::Sleep(0);        
    }
    
};



ZDEV_NAMESPACE_END

#endif //__zdev_thread_hpp__
