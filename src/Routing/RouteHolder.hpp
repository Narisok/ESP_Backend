#pragma once

#include "Forward.hpp"

#include "Binder.hpp"
#include "Callable.hpp"


#define MAX_ROUTE_SIZE (63 - (sizeof(size_t)*3))

namespace nii::routing
{
    template<class R>
    struct RouteHolder
    {
        alignas(__STDCPP_DEFAULT_NEW_ALIGNMENT__)
        uint8_t data[sizeof(size_t)*3];

        char path[MAX_ROUTE_SIZE + 1];


        inline RouteHolder(const char *path, uint8_t *callableData, size_t copySize);
        inline ~RouteHolder();


        inline R call(Binder &binder);

        // inline bool check(const char *path);

    };

    template<class R>
    RouteHolder<R>::RouteHolder(const char *path, uint8_t *callableData, size_t copySize)
        : data{}
        , path{}
    {
        char *ppath = &(this->path[0]);

        do {
            *ppath = path[0];
            ppath++;
        } while (*++path);

        std::memcpy(this->data, callableData, copySize);

        #ifdef ENABLE_LOGS
        size_t pathSize = std::strlen(this->path);
        size_t dataSize = copySize;
        std::cout << "RouteHolder >" << this->path << "< "
        << "size: " << sizeof(*this)
            << "(" <<((sizeof(size_t)*3)) << "/" << MAX_ROUTE_SIZE << ")"
        << " busy: " << (pathSize + dataSize)
            << "(" <<(pathSize) << "/" << dataSize << ")"
        << std::endl;
        #endif
    }

    template<class R>
    inline RouteHolder<R>::~RouteHolder()
    {
        #ifdef ENABLE_LOGS
        std::cout << "~RouteHolder --" << this->path << std::endl;
        #endif
        nii::util::Callable<R> *_callable = reinterpret_cast<nii::util::Callable<R>*>(&this->data);
        _callable->~Callable();
    }


    template<class R>
    R RouteHolder<R>::call(Binder &binder)
    {
        nii::util::Callable<R> *_callable = reinterpret_cast<nii::util::Callable<R>*>(&this->data);
        std::cout << "here" << std::endl;
        return _callable->call(binder);
    }
}