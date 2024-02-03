#pragma once

#include "Exceptions.hpp"

#include <iostream>
#include <utility>
#include <string>
#include <list>
#include <any>

namespace Routing
{
    template<class>
    struct RouteHolder;

    template<class>
    struct Facade;

    template<class>
    class Router;

    template<class>
    struct RouteBuilder;

    struct Binder;
    struct Parameter;

    namespace Util
    {
        template<class>
        class Callable;

        template<class, class ...>
        struct CallableHolder;

        class Path;
    }


}