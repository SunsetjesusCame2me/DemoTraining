//    线程：
//      linux下把所有的线程当成进程实现(task_struct), 不能动态增长，默认8M
//      线程栈在进程的堆区, 是从进程vm里map出来的一块内存
//      1. join():      在join的地方嗯等，等待该线程结束(比如: 在主线程创建t1，t1.join()一手，main线程就要嗯等到t1线程结束)
//      2. detach():    
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#define CASE_1

void thread_func()
{
    std::cout << "== thread func ==" << std::endl;
}

#ifdef CASE_0
    int main()
    {
        
    }
#endif

#ifdef CASE_1
    int main()
    {
        // 调用完join和detach后，线程资源都会被销毁 —— id都会变成0x0
        // 不同的地方在于，detach并不会等，该线程直接在后台运行
        // std::thread t1(thread_func);
        
        // std::cout << t1.get_id() << std::endl;  // 0x7000017cf000
        // t1.join();
        // std::cout << t1.get_id() << std::endl;  // 0x0


        std::thread t2(thread_func);

        std::cout << t2.get_id() << std::endl;  // 0x7000017cf040
        t2.detach();
        std::cout << t2.get_id() << std::endl;  // 0x0

        // 值得一提：可以用lambda表达式
        int catch_num = 1;
        std::thread t3(
            [=]() {
                std::cout << "outside catch_num is: " << catch_num << std::endl;
            }
        );
        t3.join();
        
        return 0;
    }
#endif