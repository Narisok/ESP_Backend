#pragma once

namespace nii
{
    class Response
    {
     public:
        Response();

        // virtual int code() = 0;
        virtual int code()
        {
            return 200;
        }

     private:

    };

    class NotFoundResponse: public Response
    {
     public:
        NotFoundResponse();

        inline int code() override
        {
            return 404;
        }

     private:

    };
}
