#pragma once

#include "field_proxy.hpp"

#include <soci.h>

#include <list>
#include <string>
#include <sstream>

namespace soci { namespace orm { namespace detail {

struct class_info 
{
    class_info() : base_type_(nullptr) {}

    std::string table_;

    class_info* base_type_;
    std::vector<class_info*> subtypes_;

    std::shared_ptr<statement> insert_;
    std::list<detail::field_proxy> insert_use_proxies_;
};

}}} // namespace soci, orm, detail


