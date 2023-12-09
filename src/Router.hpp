#pragma once

#include "Routing/Forward.hpp"

#include "Routing/RouteHolder.hpp"
#include "Routing/RouteBuilder.hpp"
#include "Routing/BinderCasts.hpp"

#include "Response.hpp"

#include <list>

namespace nii
{
    class Router
    {
    public:
        typedef Response* ResponseType;
        typedef routing::RouteHolder<ResponseType> Route;
        typedef routing::Binder Binder;
        typedef routing::RouteBuilder<ResponseType>::Container Container;
        // typedef std::list<Route> Container;

     public:

        inline static Container routes = {};

        // static void


        inline static routing::RouteBuilder<ResponseType> builder()
        {
            return routing::RouteBuilder<ResponseType>{&routes};
        }

        inline static Route* find(const char *path)
        {
            for (auto &route : routes) {

                if (route.check(path)) {
                    return &route;
                }

            }

            return nullptr;
        }


        inline static Binder makeBinder(const char *path, const char *mask)
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


        inline static ResponseType findCall(const char *path)
        {

            for (auto &route : routes) {

                if (route.check(path)) {

                    // std::cout << "YOUR: route --> " << route._path << std::endl;
                    // std::cout << "YOUR: path  --> " << path << std::endl;

                    Binder binder = Router::makeBinder(path, route.path);
                    try {
                        return route.call(binder);
                    } catch (...) {
                        std::cout << "FALLBACK" << std::endl;
                        continue;
                    }
                }

            }

            // #ifdef NII_ROUTING_EXCEPTIONS
            // throw NotFoundException();
            // #else
            //     throw "404 NOT FOUND";
            // #endif
                std::cout << "404 NOT FOUND" << std::endl;
            return new NotFoundResponse();
        }




    };
}