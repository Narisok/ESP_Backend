#pragma once

#include "Binder.hpp"

namespace nii::routing
{
    template<>
    struct BinderCaster<int>
    {
        inline static int cast(Parameter &&parameter)
        {
            std::cout << "To int" << std::endl;
            return std::stoi(parameter.raw());
        }
    };

    template<>
    struct BinderCaster<bool>
    {
        inline static bool cast(Parameter &&parameter)
        {
            std::cout << "To bool" << std::endl;
            return (bool) std::stoi(parameter.raw());
        }
    };

    template<>
    struct BinderCaster<std::string>
    {
        inline static std::string cast(Parameter &&parameter)
        {
            // std::cout << "To bool" << std::endl;
            return std::string(parameter.raw());
        }
    };

}