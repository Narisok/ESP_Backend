#pragma once

#include <string>
#include <ArduinoJson.h>
#include <vector>
#include <utility>

namespace nii
{
    class Response
    {
     public:
        Response();
        virtual ~Response();

        // virtual int code() = 0;
        virtual int code()
        {
            return 200;
        }

        virtual const char* contentType()
        {
            return "text/plain";
        }

        virtual const char* data()
        {
            return "OK";
        }

        virtual size_t contentLength()
        {
            return 3;
        }

        virtual std::vector<std::pair<std::string, std::string>> headers()
        {
            return {};
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

        inline const char* data() override
        {
            return "Not found";
        }

        inline size_t contentLength() override
        {
            return 10;
        }

     private:

    };

    class HtmlResponse: public Response
    {
     public:
        HtmlResponse(int code = 200, std::string data = {});

        int code() override;

        void setCode(int code);
        void setData(std::string data);
        void addData(std::string data);
        void clear();

        const char* data() override;

        size_t contentLength() override;


        inline const char* contentType() override
        {
            return "text/html";
        }

     private:

        int status_code;
        std::string cache_data;
        std::string main_data;
        std::string template_before;
        std::string template_after;
    };
    class PlainTextResponse: public Response
    {
     public:
        inline PlainTextResponse(std::string data = {})
            : main_data(data)
        {}

        inline int code() override
        {
            return 200;
        }


        inline const char* data() override
        {
            return this->main_data.c_str();
        }

        inline size_t contentLength() override
        {
            return this->main_data.size();
        }


        inline const char* contentType() override
        {
            return "text/plain";
        }

     private:
        std::string main_data;
    };

    class JsonResponse: public Response
    {
     public:
        JsonResponse(int code = 200);

        int code() override;

        void setCode(int code);
        void clearCached();

        const char* data() override;

        size_t contentLength() override;


        inline const char* contentType() override
        {
            return "application/json";
        }

        inline DynamicJsonDocument& json()
        {
            return this->doc;
        }

     private:

        int status_code;
        DynamicJsonDocument doc;
        std::string cache_data;
    };

     class RedirectResponse: public Response
    {
     public:
        inline RedirectResponse(const char *where)
            : location(where)
        {}

        inline int code() override
        {
            return 302;
        }

        inline const char* data() override
        {
            return "";
        }

        inline size_t contentLength() override
        {
            return 0;
        }


        inline const char* contentType() override
        {
            return "";
        }


        inline std::vector<std::pair<std::string, std::string>> headers() override
        {
            return {
                {"Location", this->location}
            };
        }


     private:
        std::string location;
    };
}
