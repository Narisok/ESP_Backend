#pragma once

#include "ArduinoJson.h"

#include <vector>
#include <string>
#include <fstream>

template<class T>
struct DB
{
    inline static std::vector<T> items = {};

    inline static bool cached = false;

    inline static void sync(T &elem, bool forceSave = true)
    {
        bool found = false;
        for (auto & item : DB<T>::get()) {
            if (elem.db_same(item)) {
                item = elem;
                found = true;
                break;
            }
        }

        if (! found) {
            items.push_back(elem);
        }

        if (forceSave) {
            save();
        }
    }

    inline static void remove(T &elem, bool forceSave = true)
    {
        DB<T>::get();
        typename decltype(items)::iterator iterator = items.end();

        for (auto iter = items.begin(); iter != items.end(); ++iter) {
            if (iter->db_same(elem)) {
                iterator = iter;
                break;
            }
        }

        if (iterator != items.end()) {
            items.erase(iterator);
        }

        if (forceSave) {
            save();
        }
    }

    inline static void refresh()
    {
        cached = false;
    }

    inline static void save()
    {
        DynamicJsonDocument json(2048);

        for (auto &item : items) {
            json["items"].add(item.db_write());
        }

        std::ofstream fs(T::db_table()+".db.json");

        serializeJson(json, fs);

        fs.close();
    }

    inline static  std::vector<T> & get()
    {
        if (cached) {
            return items;
        }

        items.clear();

        std::ifstream fs(T::db_table()+".db.json");

        DynamicJsonDocument json(2048);
        deserializeJson(json, fs);

        if (json["items"].is<JsonArray>()) {
            for (auto str : json["items"].as<JsonArray>()) {
                items.push_back(T::db_from(str));
            }
        }

        fs.close();


        cached = true;
        return items;
    }
};