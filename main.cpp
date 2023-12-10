// #include <iostream>

// #include <cstring>

// #include <Router.hpp>

// #include <ArduinoJson.h>
#include <Backend.hpp>

#include <iostream>
#include <fstream>
#include <vector>

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



struct Zone
{
    std::string key;
    int mode;
    int from;
    int to;

    Zone()
        : key()
        , mode(0)
        , from(0)
        , to(0)
    {}

    Zone(const Zone &other)
        : key(other.key)
        , mode(other.mode)
        , from(other.from)
        , to(other.to)
    {}

    Zone( Zone &&other)
        : key(std::move(other.key))
        , mode(other.mode)
        , from(other.from)
        , to(other.to)
    {}

    Zone(
        std::string key,
        int mode,
        int from,
        int to
    )
        : key(key)
        , mode(mode)
        , from(from)
        , to(to)
    {

    }

    Zone &operator=(const Zone &other)
    {
        this->key = other.key;
        this->mode = other.mode;
        this->from = other.from;
        this->to = other.to;

        return *this;
    }


    bool db_same(const Zone &other)
    {
        return other.key == this->key;
    }

    // void db_sync()
    // {
    //     DB<Zone>::sync(*this);
    // }

    std::string db_write()
    {
        StaticJsonDocument<200> json;

        json["key"] = this->key;
        json["mode"] = this->mode;
        json["from"] = this->from;
        json["to"] = this->to;

        std::string stream;

        serializeJson(json, stream);

        return stream;
    }

    static Zone db_from(const std::string &stream)
    {
        StaticJsonDocument<200> json;
        deserializeJson(json, stream);

        Zone zone;

        zone.key = json["key"].as<std::string>();
        zone.mode = json["mode"].as<int>();
        zone.from = json["from"].as<int>();
        zone.to = json["to"].as<int>();

        return zone;
    }

    static std::string db_table()
    {
        return "zones";
    }

};




int main()
{
    // DB<Zone>

    Zone zone("my-zone", 2, 0, 100);

    DB<Zone>::sync(zone);

    auto zones = DB<Zone>::get();

    for (auto & zone : zones) {
        printf("Zone <%s> %i  %i-%i\n", zone.key.c_str(), zone.mode, zone.from, zone.to);
        // cout << " Key: "  << zone.key << " Mode: "  << zone.mode << " From: "  << zone.from << " To: "  << zone.to << endl;
    }
    // DB<Zone>::sync(zone);

    // std::ofstream fs("images");

    // fs << 21 << endl;

    // fs.close();

    // auto zones = DB<Zone>::get();



    // DynamicJsonDocument json(2048);

    // json["items"].add(2);
    // json["items"].add(5);
    // json["items"].add(7);

    // std::string str;
    // serializeJson(json, str);

    // cout << str << endl;

    // nii::Router::builder()->path("/my-path/$")->call(foo);
    // nii::Router::builder()->path("/my-path/$")->call(bar);

    // // auto route = nii::Router::find("/my-path/234");

    // // cout << "Found: " << route << endl;

    // nii::Response *response = nii::Router::findCall("/my-path/234");

    // cout << "Response code: " << response->code() << " type: " << response->contentType() << "\ndata:" << response->data() << endl;

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