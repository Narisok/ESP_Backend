#include "Response.hpp"


namespace nii
{
    Response::Response()
    {

    }

    Response::~Response()
    {

    }


    NotFoundResponse::NotFoundResponse()
        : Response()
    {

    }

    HtmlResponse::HtmlResponse(int code, std::string data)
        : Response()
        , status_code(code)
        , cache_data()
        , main_data(std::move(data))
        , template_before()
        , template_after()
    {
        template_before = R"raw-content(<html lang="en">
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-EVSTQN3/azprG1Anm3QDgpJLIm9Nao0Yz1ztcQTwFspd3yD65VohhpuuCOmLASjC" crossorigin="anonymous">
    <title>NII BACKEND</title>
</head>
<body>
)raw-content";

        template_after = R"raw-content(
</body>
</html>
)raw-content";
    }

    int HtmlResponse::code()
    {
        return this->status_code;
    }

    void HtmlResponse::setCode(int code)
    {
        this->status_code = code;
    }

    void HtmlResponse::setData(std::string str)
    {
        this->main_data = std::move(str);

        this->clear();
    }

    void HtmlResponse::addData(std::string str)
    {
        this->main_data += std::move(str);

        this->clear();
    }

    void HtmlResponse::clear()
    {
        if (this->cache_data.size() != 0) {
        this->cache_data.clear();
        }
    }

    const char* HtmlResponse::data()
    {
         if (this->cache_data.size() == 0) {
            this->cache_data = this->template_before;
            this->cache_data += this->main_data;
            this->cache_data += this->template_after;
        }

        return this->cache_data.c_str();
    }

    size_t HtmlResponse::contentLength()
    {
        return this->cache_data.size();
    }


    JsonResponse::JsonResponse(int code)
        : Response()
        , status_code(code)
        , doc(2048)
        , cache_data()
    {

    }

    int JsonResponse::code()
    {
        return this->status_code;
    }

    void JsonResponse::setCode(int code)
    {
        this->status_code = code;
    }

    void JsonResponse::clearCached()
    {
        this->cache_data.clear();
    }

    const char* JsonResponse::data()
    {
        if (this->cache_data.size() == 0) {
            serializeJson(this->doc, this->cache_data);
        }

        return this->cache_data.c_str();
    }

    size_t JsonResponse::contentLength()
    {
        return this->cache_data.size();
    }

}
