#include <iostream>

#include <cstring>

#include <Routing/RouteHolder.hpp>
#include <Routing/Callable.hpp>

#define ENABLE_LOGS

using namespace std;

void foo(int i)
{
    cout << "Foo calls with i:" << i << endl;
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
    cout << "Hello world" << endl;
    // uint8_t data[4] = {'1', '2', '3', '4'};

    // uint8_t dest[5] = {'-', '-', '-', '-', '-'};

    // std::memcpy(&dest, &data, 5);

    uint8_t data[24]{};


    size_t copySize = sizeof(nii::util::CallableHolder(foo));
    new(data) nii::util::CallableHolder(foo);
    cout << sizeof(nii::util::CallableHolder(foo)) << endl;
    // nii::util::CallableHolder callableLambda([] (int i) {
    //     cout << "Hi lambda i:" << i << endl;
    // });



    // nii::util::CallableHolder callableController(&Controller::bar);

    nii::routing::RouteHolder<void> holder("ok-routet", data, copySize);

    holder.call(234);
    // cout << dest[0] << dest[1] << dest[2] << dest[3] << dest[4] << endl;

}