#pragma once

#include "Forward.hpp"

#include "Config.hpp"
#include "Binder.hpp"
#include "BinderCasts.hpp"

namespace nii::util
{

    template<class T>
    class Callable
    {
        protected:
        Callable() = default;

        public:
        virtual ~Callable()
        {

        }

        virtual T call(nii::routing::Binder &binder) =0;
    };


    template<class R, class ... ARGS>
    struct CallableHolder : Callable<R>
    {

    };

    // LAMBDA
    template<class L, class R, class ... ARGS>
    struct CallableHolder<L, R (ARGS ...)> : Callable<R>
    {
        L function;


        CallableHolder(L && t)
            : function(std::move(t))
        {
            #ifdef ENABLE_LOGS
            std::cout << "CallableHolder(lambda move&&)"
                << "(" << sizeof(*this) << "/" << sizeof(this->function) << ")"
            << std::endl;
            #endif
        }

        CallableHolder(const L & t)
            : function(t)
        {
            #ifdef ENABLE_LOGS
            std::cout << "CallableHolder(lambda const&)"
                << "(" << sizeof(*this) << "/" << sizeof(this->function) << ")"
            << std::endl;
            #endif
        }


        virtual ~CallableHolder()
        {
            #ifdef ENABLE_LOGS
            std::cout << "~CallableHolder(lambda)"<< std::endl;
            #endif
        }

        R call(nii::routing::Binder &binder) override
        {
            // std::cout << "CALL LAMBDA HOLDER" << std::endl;
            return function(binder_cast<ARGS>(binder.next())...);
        }
    };



    // FUNCTION
    template<class R, class ... ARGS>
    struct CallableHolder<R, R (*)(ARGS ...)> : Callable<R>
    {
        R (*function)(ARGS ...);

        CallableHolder(R (*fun)(ARGS ... args))
            : Callable<R>()
            , function(fun)
        {

            #ifdef ENABLE_LOGS
            std::cout << "CallableHolder(function)"
                << "(" << sizeof(*this) << "/" << sizeof(this->function) << ")"
            << std::endl;
            #endif
        }


        virtual ~CallableHolder()
        {
            #ifdef ENABLE_LOGS
            std::cout << "~CallableHolder(function)"<< std::endl;
            #endif
        }

        R call(nii::routing::Binder &binder) override
        {
            std::cout << "CALL F:unction HOLDER"  << std::endl;
            // std::cout << (binder_cast<ARGS>(binder.next())...) << std::endl;
            return function(binder_cast<ARGS>(binder.next())...);
        }
    };

    // CONTROLLER
    template<class R, class CONTROLLER, class ... ARGS>
    struct CallableHolder<R, R (CONTROLLER::*)(ARGS ...)> : Callable<R>
    {
        R (CONTROLLER::*function)(ARGS ...);

        CallableHolder(R (CONTROLLER::*fun)(ARGS ... args))
            : function(fun)
        {

            #ifdef ENABLE_LOGS
            std::cout << "CallableHolder(controller)"
                << "(" << sizeof(*this) << "/" << sizeof(this->function) << ")"
            << std::endl;
            #endif
        }



        virtual ~CallableHolder()
        {
            #ifdef ENABLE_LOGS
            std::cout << "~CallableHolder(controller)"<< std::endl;
            #endif
        }

        R call(nii::routing::Binder &binder) override
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


    template<class R, class ... ARGS>
    CallableHolder(R (*)(ARGS ...)) -> CallableHolder<R, R (*)(ARGS ...)>;


    template<class R, class CONTROLLER, class ... ARGS>
    CallableHolder(R (CONTROLLER::*)(ARGS ...)) -> CallableHolder<R, R (CONTROLLER::*)(ARGS ...)>;

}