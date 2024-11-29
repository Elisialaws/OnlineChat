#ifndef SINGLETON_H
#define SINGLETON_H
#include"global.h"
#include <memory>
#include <mutex>
#include <iostream>
#include "global.h"

using namespace std;
template <typename T>
class Singleton {
protected:
    Singleton() = default;
    Singleton(const Singleton<T>&) = delete;
    Singleton& operator=(const Singleton<T>& st) = delete;

    static std::shared_ptr<T> _instance;
public:
    /**
     * @brief GetInstance
     * @return
     * once_flag确保某一代码块在多线程下仅被执行一次
     */
    static std::shared_ptr<T> GetInstance() {
        static std::once_flag s_flag;
        std::call_once(s_flag, [&]() {
            _instance = shared_ptr<T>(new T);
            });

        return _instance;
    }
    void PrintAddress() {
        std::cout << _instance.get() << endl;
    }
    ~Singleton() {
        std::cout << "this is singleton destruct" << std::endl;
    }
};

template <typename T>
std::shared_ptr<T> Singleton<T>::_instance = nullptr;
#endif // SINGLETON_H
