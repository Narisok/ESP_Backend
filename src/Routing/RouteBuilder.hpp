#pragma once

#include "Forward.hpp"

#include "RouteHolder.hpp"
#include "Callable.hpp"

#include <list>

namespace nii::routing
{

    template<class R>
    struct RouteBuilder
    {
        typedef RouteHolder<R> Route;
        typedef std::list<Route> Container;

        alignas(__STDCPP_DEFAULT_NEW_ALIGNMENT__)
        uint8_t data[sizeof(size_t)*3];
        std::string _path;

        nii::util::Callable<R> *_callable;

        Container *container;

        size_t copy_size;


        inline RouteBuilder(Container *container);

        inline ~RouteBuilder();

        inline RouteBuilder* operator->()
        {
            return this;
        }

        inline RouteBuilder* path(const std::string& path)
        {
            this->_path += path;

            return this;
        }


        template<class T>
        inline RouteBuilder* call(T && callable)
        {
            // static_assert(sizeof(Util::CallableHolder(std::forward<T>(callable))) <= 16, "BAd size");
             if constexpr (sizeof(nii::util::CallableHolder(std::forward<T>(callable))) > (sizeof(size_t)*3)) {
                std::cout << "BAD SIZE" << std::endl;
            }

            this->copy_size = sizeof(nii::util::CallableHolder(std::forward<T>(callable)));

            auto pointer = new(data) nii::util::CallableHolder(std::forward<T>(callable));

            auto dynamic = dynamic_cast<nii::util::Callable<R>*>(pointer);

            this->_callable  = dynamic;

            // std::cout << "this->_callable: " << this->_callable << " pointer: " << pointer << " dynamic: " << dynamic << std::endl;
            // std::cout << "n: " << typeid(_callable).name() << " p: " << typeid(pointer).name() << std::endl;

            return this;
        }

        // template<>
        // inline RouteBuilder* call<void>(std::function<void()> callable)
        // {
        //     _callable = new Util::CallableHolder(std::move(callable));

        //     return this;
        // }

        // template<class T>
        // RouteBuilder* call(std::function<T()>  callable)
        // {
        //     _callable = new Util::CallableHolder(callable);

        //     return this;
        // }
    };

    template<class R>
    RouteBuilder<R>::RouteBuilder(Container *container)
            : data{}
            , _path("")
            , _callable(nullptr)
            , container(container)
            , copy_size(0)
        {
            std::cout << "RouteBuilder()" << " path: " << _path << std::endl;
        }

    template<class R>
    RouteBuilder<R>::~RouteBuilder()
    {


        if (_callable) {

            // std::cout << "ADD NEW ROUTE" << std::endl;
             // TODO MOVE MEMORY CALLABLE

            this->container->emplace_back(RouteHolder<R>(this->_path.c_str(), this->data, this->copy_size));
            // _callable->call();

            // _callable->~Callable();
        } else {
            // std::cout << "NO CALL" << std::endl;
        }

std::cout << "~RouteBuilder() " << this->_callable << std::endl;
        // std::cout << "~ path: " << _path << std::endl;
    }

}