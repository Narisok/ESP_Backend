#include <iostream>

#include <cstring>

#include <Router.hpp>

#define ENABLE_LOGS

using namespace std;

nii::Response* foo(int i)
{
    cout << "Foo calls with i:" << i << endl;

    return new nii::Response();
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
    nii::Router::builder()->path("/my-path/$")->call(foo);

    // auto route = nii::Router::find("/my-path/234");

    // cout << "Found: " << route << endl;

    nii::Response *response = nii::Router::findCall("/my-path/234");

    cout << "Response code: " << response->code() << endl;
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