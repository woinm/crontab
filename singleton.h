#ifndef SINGLETON_H
#define SINGLETON_H

template <typename T>
class Singleton {
    struct object_creator {
        object_creator() {
            Singleton<T>::instance();
        }
        inline void do_nothing() const {}
    };

    static object_creator create_object;

public:
    typedef T object_type;

    static object_type& instance() {
        static object_type obj;
        // do_nothing是确保create_object构造函数被调用,这跟模板的编译有关
        create_object.do_nothing();
        return obj;
    }
};

template <typename T>
typename Singleton<T>::object_creator Singleton<T>::create_object;
#define DEF_GET_INSTANCE(classname) classname::instance()

#endif // SINGLETON_H
