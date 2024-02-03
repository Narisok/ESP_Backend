#pragma once

#include "RouteForward.hpp"


namespace Routing
{
    struct Parameter
    {
        const char *start;
        size_t end;

        inline Parameter(const char *start, size_t end)
                : start(start)
                , end(end)
        {
            
        }

        inline std::string raw()
        {
            return std::string(start, &start[end]);
        }
    };


    struct Binder
    {
        const char *path;
        const std::string *mask;

        size_t mask_index;
        size_t path_index;

        inline Binder(const char *path, const std::string *mask)
                : path(path)
                , mask(mask)
                , mask_index(0)
                , path_index(0)
        {

        }

        inline Parameter next()
        {

            size_t start_path = 0;
            for (size_t i = mask_index; i < mask->size(); i++) {
                if ((*mask)[i] == '$') {
                    mask_index = i + 1;
                    break;
                }
                start_path++;
            }
            std::cout << "start path: " << start_path << std::endl;
            size_t length = 0;
            const char endSymbol = mask_index < mask->size() ? (*mask)[mask_index] : '/';

            const char *start = path + path_index + start_path;
            const char *startPar = path + path_index + start_path;
            while (*start && *start != endSymbol) {
                length++;
                start++;
            }

            path_index += length + start_path;

            std::cout << "path_index: " << path_index << std::endl;
            std::cout << "mask_index: " << mask_index << std::endl;
            std::cout << std::endl;


            return Parameter(startPar, length);
        }

        // inline Parameter operator[](size_t index)
        // {
        //     return Parameter();
        //     // return parameters[index];
        // }
    };


    template<class T>
    struct BinderCaster
    {
        inline static T cast(Parameter &&)
        {
            throw "Binder cast not defined for this type";
        }
    };


    template<class T>
    T binder_cast(Parameter parameter)
    {
        std::cout << "Raw: " << parameter.raw() << std::endl;
        return BinderCaster<T>::cast(std::move(parameter));
        // return T{};
    }


    template<>
    struct BinderCaster<int>
    {
        inline static int cast(Parameter &&parameter)
        {
            return std::stoi(parameter.raw());
        }
    };

    template<>
    struct BinderCaster<bool>
    {
        inline static bool cast(Parameter &&parameter)
        {

            return (bool) std::stoi(parameter.raw());
        }
    };



}