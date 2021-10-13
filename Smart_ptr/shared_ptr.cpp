#include <iostream>

// shared_ptr就是加了一个【引用计数】，为0的时候析构
int main()
{
    // 初始化方式1
    std::shared_ptr<int > p1(new int(1));
    
    // 初始化方式2
    {
        std::shared_ptr<int > p2 = p1;
        std::cout << "use count: " << p1.use_count() << std::endl;
    }

    std::cout << "use count: " << p1.use_count() << std::endl;
    
    // 这种初始化方式不行
    // int *p = new int(1);
    // std::shared_ptr<int > p3 = p;
    
    return 0;
}