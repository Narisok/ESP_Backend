#pragma once

#include "Backend.hpp" // Бібліотека Backend framework (MVC based)

#include <ESP8266WebServer.h>   // вебсерве HTTP (TCP/IP)
#include <Arduino.h>            // Середовище Arduino IDE

#include <FS.h>        // Файлова система
#include <LittleFS.h>  // Драйвер файлової системи

#include "Response.hpp"


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

        inline void setup(
            const char *ssid = nullptr,
            const char *passPhrase = nullptr,
            void (*defineRoutes)() = nullptr
        )
        {
            delay(1000);
            Serial.begin(115200);

            TRACE("Serial mounted...\n");
            delay(1000);

            TRACE("Start up..\n");

            WiFi.mode(WIFI_STA);
            if (strlen(ssid) == 0) {
                WiFi.begin();
            } else {
                WiFi.begin(ssid, passPhrase);
            }

            TRACE("Mounting the filesystem...\n");
            if (!LittleFS.begin()) {
                TRACE("could not mount the filesystem...\n");
                delay(2000);
                ESP.restart();
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