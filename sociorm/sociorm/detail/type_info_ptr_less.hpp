#pragma once

#include <typeinfo>

namespace soci { namespace orm { namespace detail {

struct type_info_ptr_less
{
    bool operator ()(const std::type_info* a, const std::type_info* b) const 
    {
        return !!a->before(*b);
    }
};

}}}
