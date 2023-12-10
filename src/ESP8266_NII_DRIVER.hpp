#pragma once

#include "Backend.hpp"

#include <ESP8266WebServer.h>
#include <Arduino.h>



#ifndef TRACE
#define TRACE(...) Serial.printf(__VA_ARGS__)
#endif


namespace nii
{
    ESP8266WebServer server(80);

    class Driver : public RequestHandler
    {
        Router::Route * ref;
        ESP8266WebServer * server;
    public:

        inline Driver(ESP8266WebServer * server)
        : server(server)
        , ref(nullptr)
        {

        }

        inline bool canHandle(HTTPMethod method, const String& uri)  override
        {
            return true;
            // this->ref = Routing::Router<Response>::find(uri.c_str());
            // return this->ref != nullptr;
        }


    inline bool handle(ESP8266WebServer &server, HTTPMethod requestMethod, const String &requestUri) override
    {

        Response * response = Router::findCall(requestUri.c_str());

        for (auto [name, value] : response->headers()) {
            this->server->sendHeader(name.data(), value.data());
        }

        this->server->send(response->code(), response->contentType(), response->data(), response->contentLength());  // all done.

        delete response;

        return (true);
    }  // handle()

    };


    namespace backend
    {

        inline void setup(const char *ssid = nullptr, const char *passPhrase = nullptr, void (*defineRoutes)() = nullptr)
        {
            delay(1000);
            Serial.begin(115200);
            delay(1000);
            TRACE("Start up..\n");

            WiFi.mode(WIFI_STA);
            if (strlen(ssid) == 0) {
                WiFi.begin();
            } else {
                WiFi.begin(ssid, passPhrase);
            }

            TRACE("Connecting to WiFi...\n");
            while (WiFi.status() != WL_CONNECTED) {
                delay(500);
                TRACE(".");
            }
            TRACE("connected.\n");

            server.addHandler(new Driver(&server));

            server.enableCORS(true);

            if (defineRoutes) {
                defineRoutes();
            }

            server.begin();
            TRACE("hostname=%s\n", WiFi.getHostname());


            TRACE("server_url=%s\n", WiFi.localIP().toString().c_str());
        }

        inline void update()
        {
            server.handleClient();
        }

    }
}