#pragma once

#include "RouteForward.hpp"

#include "RouteBuilder.hpp"




namespace Routing
{
    template<class R>
    struct RouteHolder
    {
        alignas(__STDCPP_DEFAULT_NEW_ALIGNMENT__)
        uint8_t data[sizeof(size_t)*3];

        // char path[MAX_ROUTE_SIZE];

        std::string _path;
        // // Util::Callable<R> *_callable;


        inline RouteHolder(const std::string &path, uint8_t *callableData);
        inline ~RouteHolder();


        inline R call(Binder&);

        inline bool check(const char *path);

    };



    template<class R>
    RouteHolder<R>::RouteHolder(const std::string &path, uint8_t *callableData)
        : _path(path)
        // , data(callable)
    {
        std::memcpy(&data, callableData, sizeof(size_t)*3);
        std::cout << "RouteHolder (const std::string &path, Util::Callable<R> *callable) " << (void*)callableData << "|" << &data << "|" << this << std::endl;
    }

    template<class R>
    inline RouteHolder<R>::~RouteHolder()
    {
        Util::Callable<R> *_callable = reinterpret_cast<Util::Callable<R>*>(&data);
        _callable->~Callable();
        std::cout << "~RouteHolder " << this << "|" << std::endl;
    }


    template<class R>
    R RouteHolder<R>::call(Binder &binder)
    {
        Util::Callable<R> *_callable = reinterpret_cast<Util::Callable<R>*>(&data);
        return _callable->call(binder);
    }

    template<class R>
    bool RouteHolder<R>::check(const char *path)
    {
        size_t i, j;

        for (i = 0, j = 0; i < _path.size(); i++, j++) {

            if (path[j] == '\0') {
                return false;
            }

            if (_path[i] == '$') {
                i++;
                const char end_arg = i < _path.size() ? _path[i] : '/';

                do {
                    j++;
                } while( path[j+1] && path[j] != end_arg );


            } else {

                if (_path[i] != path[j]) {
                    return false;
                }

            }

        }


        if (path[j] != '\0') {
            return false;
        }

        return true;
    }

}
