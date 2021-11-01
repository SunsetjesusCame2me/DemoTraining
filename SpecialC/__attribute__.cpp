/*     GNUC的一个机制，用来给编译器一些信息
*    1. 函数属性    2. 变量属性     3.类型属性
*    语法：__attribute__ ((attribute-list))
*/
#include <iostream>
#include <cstdio>
// #define CASE_1
#define CASE_2

// 函数属性
#ifdef CASE_1
    // constructor 让函数自动被调用，在main之前
    
    // 优先级： 1～100默认保留， 100之后是优先级, 越小越优先
    __attribute__((constructor(101))) void before_main_1()
    {
        std::cout << "== before main 1 ==" << std::endl;
    }   
    __attribute__((constructor(102))) void before_main_2()
    {
        std::cout << "== before main 2 ==" << std::endl;
    }

    // destructor 在main之后被调用
    __attribute__((destructor)) void after_main()
    {
        std::cout << "== after main ==" << std::endl;
    }

    // 这个函数并不会返回
    __attribute__((noreturn)) void noreturn_func()
    {
        return ;
    }
    int main()
    {
        std::cout << "   [main]   " << std::endl;
        return 0;
    }
#endif

// 变量属性: 放在变量前面修饰
#ifdef CASE_2
    // aligned: 改变分配内存时的对齐规则
    struct test1 
    {
        char a;
        char b;
    } __attribute__((aligned(8))) st1;
    
    int main()
    {
        // 原本应该是2 但是设置了8字节对齐 只能补齐到8
        std::cout << sizeof(st1) << std::endl;

        return 0;
    }
#endif