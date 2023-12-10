#include <iostream>

#include <cstring>

#include <Router.hpp>

#include <ArduinoJson.h>

#define ENABLE_LOGS

using namespace std;

nii::Response* foo(int i)
{
    auto res = new nii::JsonResponse();

    res->json()["sensor"] = "gps";
    res->json()["time"]   = 1351824120;
    res->json()["data"][0] = 48.756080;
    res->json()["data"][1] = 2.302038;


    return res;
}


nii::Response* bar(int i)
{
    auto res = new nii::HtmlResponse();

    res->setData(R"raw-data(
        <H1>Hello</H1>
    )raw-data");

    res->addData(R"raw-data(
        <div class="label label-sm label-primary">main</div>
    )raw-data");

    return res;
}

struct Controller
{
    void bar(int i)
    {
        cout << "Controller bar i:" << i << endl;
    }
};

int main()
{
    // nii::Router::builder()->path("/my-path/$")->call(foo);
    nii::Router::builder()->path("/my-path/$")->call(bar);

    // auto route = nii::Router::find("/my-path/234");

    // cout << "Found: " << route << endl;

    nii::Response *response = nii::Router::findCall("/my-path/234");

    cout << "Response code: " << response->code() << " type: " << response->contentType() << "\ndata:" << response->data() << endl;

    // DynamicJsonDocument doc(1024);
    // std::string str;
    // serializeJson(doc, str);

    // cout << "JSON:" << res->data() << ":" << endl;
    // nii::util::CallableHolder callableController(&Controller::bar);

//     nii::routing::RouteHolder<void> holder("ok-routet/$", data, copySize);

// if (holder.check("ok-routet/5")) {
//     cout << "OK" << endl;
// } else {
//     cout << "NOT OK" << endl;

// }

//     nii::routing::Binder binder("ok-routet/2", holder.path);
    // cout << "Binder param: " <<  binder.next().raw() << endl;
    // holder.call(binder);
    // cout << dest[0] << dest[1] << dest[2] << dest[3] << dest[4] << endl;

}