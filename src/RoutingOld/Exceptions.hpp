#if defined(NII_EXCEPTIONS) && defined(NII_EXCEPTIONS_ENABLED)

    #ifndef NII_ROUTING_EXCEPTIONS // GUARD
    #define NII_ROUTING_EXCEPTIONS

    #include <Exceptions/Exception>


    // Routing exceptions below
    namespace Routing
    {

        class FallbackException : public Exception
        {
        public:
            inline virtual const char* what() const noexcept override
            {
                return "The route was found; Parameters substitution failed;";
            }
        };

        class NotFoundException : public Exception
        {
        public:
            inline virtual const char* what() const noexcept override
            {
                return "The route not found";
            }
        };

        // class NotFoundException : public Exception
        // {
        // public:
        //     inline virtual const char* what() const noexcept override
        //     {
        //         return "The route not found";
        //     }
        // };

    }

    #endif // END GUARD

#endif