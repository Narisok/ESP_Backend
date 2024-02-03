#pragma once

#include "RouteForward.hpp"

#include "Router.hpp"
#include "Route.hpp"
#include "Binder.hpp"


namespace Routing
{
    namespace Util
    {

        template<class T>
        class Callable
        {
            // Callable(T && t)
            // {

            // }

            // Callable(Callable*)
            // {

            // }
            // // Callable() = delete;
            // Callable& operator= (Callable *)
            // {

            // }

         protected:
            Callable() = default;

            // Callable& operator=(Callable) = default;

         public:
            virtual ~Callable()
            {

            }

            virtual T call(Binder&) =0;
        };

        // template<class T>
        // Callable(std::function< T()> ) -> Callable<T>;

        // Callable(std::function<void()>) -> Callable<void>;



        template<class R, class ... ARGS>
        struct CallableHolder : Callable<R>
        {

            // R call(Binder& ) override
            // {
            //     std::cout << "CALL EMPTY HOLDER" << std::endl;
            //     #ifdef NII_ROUTING_EXCEPTIONS
            //     throw NotFoundException();
            //     #else
            //         throw "404 NOT FOUND";
            //         std::cout << "404 NOT FOUND" << std::endl;
            //     #endif
            // }
        };

        template<class L, class R, class ... ARGS>
        struct CallableHolder<L, R (ARGS ...)> : Callable<R>
        {
            L function;


            CallableHolder(L && t)
                : function(std::move(t))
            {
                std::cout << "CH lambda holder   " << " ::" << sizeof(*this) << "::" << sizeof(function) << ":: " << std::endl;
            }

            CallableHolder(const L & t)
                : function(t)
            {
                std::cout << "CH lambda holder   " << " ::" << sizeof(*this) << "::" << sizeof(function) << ":: " << std::endl;
                // std::cout << "CH T   " << " ::" << sizeof(*this) << ":: " << std::endl;
            }

            R call(Binder& binder) override
            {
                std::cout << "CALL LAMBDA HOLDER" << std::endl;
                return function(binder_cast<ARGS>(binder.next())...);
            }
        };



        template<class R, class ... ARGS>
        struct CallableHolder<R, R (*)(ARGS ...)> : Callable<R>
        {
            R (*function)(ARGS ...);

            CallableHolder(R (*fun)(ARGS ... args))
                : Callable<R>()
                , function(fun)
            {
                std::cout << "CH fun" << " ::" << sizeof(*this) << ":: " << std::endl;
            }

            R call(Binder& binder) override
            {
                std::cout << "CALL F:unction HOLDER"  << std::endl;
                return function(binder_cast<ARGS>(binder.next())...);
            }
        };

        template<class R, class CONTROLLER, class ... ARGS>
        struct CallableHolder<R, R (CONTROLLER::*)(ARGS ...)> : Callable<R>
        {
            R (CONTROLLER::*function)(ARGS ...);

            CallableHolder(R (CONTROLLER::*fun)(ARGS ... args))
                : function(fun)
            {
                std::cout << "CallableHolder R (CONTROLLER::*fun)(ARGS ... args) " << " ::" << sizeof(*this) << ":: " << this  << std::endl;
            }



            virtual ~CallableHolder()
            {
                std::cout << "~CallableHolder R (CONTROLLER::*fun)(ARGS ... args) " << " ::" << sizeof(*this) << ":: " << this  << std::endl;
            }

            R call(Binder& binder) override
            {
                std::cout << "CALL C:ontroller HOLDER " << this << std::endl;
                auto controller = CONTROLLER{};
                return (controller.*function)(binder_cast<ARGS>(binder.next())...);
            }

        private:
            CallableHolder(const CallableHolder<R, R (CONTROLLER::*)(ARGS ...)> &other)
                : function(other.fun)
            {
            }

            CallableHolder(CallableHolder<R, R (CONTROLLER::*)(ARGS ...)> &&other)
                : function(other.fun)
            {
            }
        };



        template<class T>
        struct Deductor;

        template<class R, class C, class ...ARGS>
        struct Deductor<R (C::*) (ARGS...)> { using type = R(ARGS...); };
        template<class R, class C, class ...ARGS>
        struct Deductor<R (C::*) (ARGS...) const> { using type = R(ARGS...); };
        template<class R, class C, class ...ARGS>
        struct Deductor<R (C::*) (ARGS...) volatile> { using type = R(ARGS...); };
        template<class R, class C, class ...ARGS>
        struct Deductor<R (C::*) (ARGS...) const volatile> { using type = R(ARGS...); };

        template<class R, class C, class ...ARGS>
        struct Deductor<R (C::*) (ARGS...) &> { using type = R(ARGS...); };
        template<class R, class C, class ...ARGS>
        struct Deductor<R (C::*) (ARGS...) const &> { using type = R(ARGS...); };
        template<class R, class C, class ...ARGS>
        struct Deductor<R (C::*) (ARGS...) volatile &> { using type = R(ARGS...); };
        template<class R, class C, class ...ARGS>
        struct Deductor<R (C::*) (ARGS...) const volatile &> { using type = R(ARGS...); };

        template<class R, class C, class ...ARGS>
        struct Deductor<R (C::*) (ARGS...) noexcept> { using type = R(ARGS...); };
        template<class R, class C, class ...ARGS>
        struct Deductor<R (C::*) (ARGS...) const noexcept> { using type = R(ARGS...); };
        template<class R, class C, class ...ARGS>
        struct Deductor<R (C::*) (ARGS...) volatile noexcept> { using type = R(ARGS...); };
        template<class R, class C, class ...ARGS>
        struct Deductor<R (C::*) (ARGS...) const volatile noexcept> { using type = R(ARGS...); };

        template<class R, class C, class ...ARGS>
        struct Deductor<R (C::*) (ARGS...) & noexcept> { using type = R(ARGS...); };
        template<class R, class C, class ...ARGS>
        struct Deductor<R (C::*) (ARGS...) const & noexcept> { using type = R(ARGS...); };
        template<class R, class C, class ...ARGS>
        struct Deductor<R (C::*) (ARGS...) volatile & noexcept> { using type = R(ARGS...); };
        template<class R, class C, class ...ARGS>
        struct Deductor<R (C::*) (ARGS...) const volatile & noexcept> { using type = R(ARGS...); };



        template<class R, class T = typename Deductor<decltype(&R::operator())>::type>
        CallableHolder( R ) -> CallableHolder<R, T>;


        // template<class R, class ... ARGS>
        // CallableHolder(R(ARGS...)) -> CallableHolder<R, ARGS...>;

        // template<class R>
        // CallableHolder(std::function<R()>) -> CallableHolder<R, std::function<R()>>;

        template<class R, class ... ARGS>
        CallableHolder(R (*)(ARGS ...)) -> CallableHolder<R, R (*)(ARGS ...)>;


        template<class R, class CONTROLLER, class ... ARGS>
        CallableHolder(R (CONTROLLER::*)(ARGS ...)) -> CallableHolder<R, R (CONTROLLER::*)(ARGS ...)>;






    }

    /*
    *
    * RouteBuilder
    * -- url --
    * -> path
    *
    * -- alliance --
    * -> group
    *
    * -- methods --
    * -> method
    * ->* get
    * ->* post
    * ->* put
    * ->* patch
    * ->* delete
    * ->* options
    * ->* head
    *
    * -- calling --
    * -> call
    */
    template<class R>
    struct RouteBuilder
    {
        alignas(__STDCPP_DEFAULT_NEW_ALIGNMENT__)
        uint8_t data[sizeof(size_t)*3];
        std::string _path;

        Util::Callable<R> *_callable;



        inline char hello() {return 'H';}

        inline RouteBuilder();

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
             if constexpr (sizeof(Util::CallableHolder(std::forward<T>(callable))) > 24) {
                std::cout << "BAD SIZE" << std::endl;
            }
            std::cout << "SIZEOF: "  <<  sizeof(Util::CallableHolder(std::forward<T>(callable))) << std::endl;

            auto pointer = new(data) Util::CallableHolder(std::forward<T>(callable));

            std::cout << pointer << " | " << &data << std::endl;
            auto dynamic = dynamic_cast<Util::Callable<R>*>(pointer);

            _callable  = dynamic;
            std::cout << "n: " << typeid(_callable).name() << " p: " << typeid(pointer).name() << std::endl;

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
    RouteBuilder<R>::RouteBuilder()
            : _path("")
            , _callable(nullptr)
        {
            // std::cout << "RouteBuilder()" << " path: " << _path << std::endl;
        }

    template<class R>
    RouteBuilder<R>::~RouteBuilder()
    {
        std::cout << "~RouteBuilder()" << std::endl;

        if (_callable) {
            std::cout << "OKIIIII: " <<  this << std::endl;
            ; // TODO MOVE MEMORY CALLABLE
            Router<R>::routes.emplace_back(RouteHolder<R>(_path, data));
            // _callable->call();

            // _callable->~Callable();
        } else {
            std::cout << "NO CALL" << std::endl;
        }


        std::cout << "~ path: " << _path << std::endl;
    }

}