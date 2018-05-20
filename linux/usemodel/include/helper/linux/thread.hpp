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
    /** �߳������� */
    virtual void run() = 0;
    
    /** �����߳� */
    void start() {
        pthread_create(&th, NULL, thread_function, (void *)this);
    }
    
    /** ǿ���˳��߳� */
    void kill() {
        pthread_cancel(th);
    }
    
    /** �ȴ��߳̽��� */
    void join() {
        pthread_join(th, NULL);
    }
    
    /** ���� */
    static void sleep(unsigned long milliseconds) {
        timespec ts;
        ts.tv_sec = (time_t)(milliseconds / 1000);
        ts.tv_nsec = (milliseconds % 1000) * 1000 * 1000;
        nanosleep(&ts, 0);
    }

    /** ���ص�ǰ�߳� ID */
    static ulong getCurrentId() {
        return (ulong) pthread_self();
    }
    
    /** ���� cpu ʱ��Ƭ */
    static void yield() {
        sched_yield();        
    }
  
};


ZDEV_NAMESPACE_END

#endif //__zdev_thread_hpp__
