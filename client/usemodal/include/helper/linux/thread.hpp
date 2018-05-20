#ifndef __zdev_thread_hpp__
#define __zdev_thread_hpp__

#include "../sys/zbase.hpp"
#include <pthread.h>
#include <sched.h>
#include <time.h>

ZDEV_NAMESPACE_BEGIN

class Thread {
private:
    pthread_t th;
    static void *thread_function(void *self) { ((Thread *)self)->run(); }
public:
    /** 线程运行体 */
    virtual void run() = 0;
    
    /** 启动线程 */
    void start() {
        pthread_create(&th, NULL, thread_function, (void *)this);
    }
    
    /** 强制退出线程 */
    void kill() {
        pthread_cancel(th);
    }
    
    /** 等待线程结束 */
    void join() {
        pthread_join(th, NULL);
    }
    
    /** 休眠 */
    static void sleep(unsigned long milliseconds) {
        timespec ts;
        ts.tv_sec = (time_t)(milliseconds / 1000);
        ts.tv_nsec = (milliseconds % 1000) * 1000 * 1000;
        nanosleep(&ts, 0);
    }

    /** 返回当前线程 ID */
    static ulong getCurrentId() {
        return (ulong) pthread_self();
    }
    
    /** 放弃 cpu 时间片 */
    static void yield() {
        sched_yield();        
    }
  
};


ZDEV_NAMESPACE_END

#endif //__zdev_thread_hpp__
