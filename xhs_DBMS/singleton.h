#ifndef SINGLETON_H
#define SINGLETON_H
#include <global.h>

/**
 * @brief The Singleton class
 * 单例类作为基类去使用
 * 利用的是 C++ 的 CRTP 奇异递归模板
 */
template <typename T>
class Singleton{

protected:
    /**
     * @brief Singleton构造函数
     * @note 设置成protected是为了子类继承的时候可以构造Singleton
     */
    Singleton() = default;

    /**
     * @brief 单例模式中删除拷贝构造
     */
    Singleton(const Singleton<T>&) = delete;

    /**
     * @brief 单例模式中删除赋值拷贝构造
     */
    Singleton& operator=(const Singleton<T>& st) = delete;

    /**
     * @brief _instance 智能指针来管理实例
     * @note
     * 使用智能指针来管理就不需要拷贝构造了
     */
    static std::shared_ptr<T> _instance;

public:
    /**
     * @brief 得到单例模式类的实例
     * @return std::shared_ptr<T>
     * @details
     * 利用 once_flag 和 call_once 这个组合
     * 当第一次调用函数的时候 once_flag 就会被初始化内部标记为 false
     * 当调用了call_once, once_flag 内部标记就会变成 true
     * 再次调用函数, call_once 发现 s_flag 内部标记为 true, 则不调用函数
     * 确保函数只被调用一次
     */
    static std::shared_ptr<T> getInstance() {
        static std::once_flag s_flag;
        std::call_once(s_flag, [&](){
            _instance = std::shared_ptr<T>(new T);
            return _instance;
        });
    }

    /**
     * @brief 打印 _instance 的地址
     */
    void printAddress() {
        std::cout << _instance.get() << std::endl;
    }

    ~Singleton() {
        std::cout << "this is singleton destruct" << std::endl;
    }
};

//_instance 初始化为 nullptr
//在 getInstance 方法中, 当第一次调用时, 如果 _instance 是 nullptr
//那么它将被初始化为一个新的 T 类型对象的 shared_ptr。
template <typename T>
std::shared_ptr<T> Singleton<T>::_instance = nullptr;

#endif // SINGLETON_H
