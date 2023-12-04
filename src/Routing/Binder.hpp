#pragma once

#include "Forward.hpp"


namespace nii::routing
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
        const char *mask;

        size_t mask_index;
        size_t path_index;

        inline Binder(const char *path, const char *mask)
                : path(path)
                , mask(mask)
                , mask_index(0)
                , path_index(0)
        {

        }

        inline Parameter next()
        {

            size_t start_path = 0;
            size_t maskSize = strlen(this->mask);
            for (size_t i = mask_index; i < maskSize; i++) {
                if (mask[i] == '$') {
                    mask_index = i + 1;
                    break;
                }
                start_path++;
            }

            size_t length = 0;
            const char endSymbol = mask_index < maskSize ? mask[mask_index] : '/';

            const char *start = path + path_index + start_path;
            const char *startPar = path + path_index + start_path;
            while (*start && *start != endSymbol) {
                length++;
                start++;
            }

            path_index += length + start_path;

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
            std::cout << "None" << std::endl;
            throw "Binder cast not defined for this type";
        }
    };


    template<class T>
    T binder_cast(Parameter parameter)
    {
        return BinderCaster<T>::cast(std::move(parameter));
    }




}