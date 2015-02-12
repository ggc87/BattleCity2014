#ifndef SINGLETON_H
#define SINGLETON_H


template <class T>
class Singleton{
public:
    Singleton(){}
    static T* getPtr(){
        static T mInstance;
        return &mInstance;
    }
};

#endif // SINGLETON_H
