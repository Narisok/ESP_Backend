#pragma once

#include "RouteForward.hpp"

#include "Route.hpp"
#include "Binder.hpp"

namespace Routing
{
    template<class R>
    class Router
    {
    public:
        typedef RouteHolder<R> Route;
        typedef std::list<Route> Container;

     public:

        inline static Container routes = {};

        // static void

        inline static Route* find(const char *path)
        {
            for (auto &route : routes) {

                if (route.check(path)) {
                    return &route;
                }

            }

            return nullptr;
        }


        inline static Binder makeBinder(const char *path, const std::string *mask)
        {
            return Binder(path, mask);
        }

        // inline static R findCall(const char *path)
        // {
        //     auto route = Router::find(path);

        //     std::cout << "YOUR: route --> " << (route ? route->_path : "nullptr") << std::endl;
        //     std::cout << "YOUR: path  --> " << path << std::endl;

        //     if (route) {
        //         Binder binder = Router::makeBinder(path, &route->_path);
        //         return route->call(binder);
        //     }
        //     std::cout << "404 NOT FOUND" << std::endl;
        //     // return {};
        // }


        inline static R findCall(const char *path)
        {

            for (auto &route : routes) {

                if (route.check(path)) {

                    std::cout << "YOUR: route --> " << route._path << std::endl;
                    std::cout << "YOUR: path  --> " << path << std::endl;

                    Binder binder = Router::makeBinder(path, &route._path);
                    try {
                        return route.call(binder);
                    } catch (...) {
                        std::cout << "FALLBACK" << std::endl;
                        continue;
                    }
                }

            }

            #ifdef NII_ROUTING_EXCEPTIONS
            throw NotFoundException();
            #else
                throw "404 NOT FOUND";
                std::cout << "404 NOT FOUND" << std::endl;
            #endif
        }




    };
}