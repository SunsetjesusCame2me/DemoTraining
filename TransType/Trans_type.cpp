#include <iostream>

class Father
{
    public:
        virtual void out(){}
    private:
        int num;
};

class Son : public Father
{
    public:
        void son_out(){ std::cout << "1" << std::endl;}
    private:
        int son_num;
};

int main()
{
    //                         static_cast && dynamic_cast 
    //          static_cast:               ｜           dynamic_cast:
    //    编译时期强转, 不提供运行时检查        |          运行时期强转，更安全： [转换失败返回nullptr]
    //     多用于 [基本数据类型] 转换          |        多用于 [对象指针] 转换和 [void* -> anytype*]
    //      编码时就要确认是否安全             |        运行时检查是否合法，开销大点: 在运行check时，检查的 [vptr] 中的类型信息
    
    int a = 10;
    double b = static_cast<double>(a);
    char c = static_cast<char>(a);

    Father *fa_ptr = new Father;
    Son   *son_ptr = new Son;
    
    // 上行转换: 很安全,把子类多出的部分舍弃就OK
    Father *p1 = dynamic_cast<Father*>(son_ptr);

    // 下行转换: 父向子转换是不允许的，所以转换结果为nullptr
    Son *p2 = dynamic_cast<Son*>(fa_ptr);
    if (p2 == nullptr)
    {
        std::cout << "convert failed" << std::endl;
    }
    else
    {
        p2->son_out();
    }

    //                          reinterpret_cast
    //                  非常激进的转换，编译时完成，单纯按位的裸形式转换，很危险，除非知道要干嘛
    char *pc = reinterpret_cast<char*>(son_ptr); 

    return 0;
}