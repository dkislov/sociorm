#pragma once

#include "field_proxy.hpp"

#include <soci.h>

#include <boost/any.hpp>

#include <list>
#include <string>
#include <sstream>

namespace soci { namespace orm { namespace detail {

struct class_info 
{
    std::shared_ptr<statement> insert_;
    std::list<boost::any> insert_use_proxies_;
};

struct collect_names_and_exchange_action
{
    collect_names_and_exchange_action(
        statement& s
      , std::list<boost::any>& proxies
      ) 
      : s_(s) 
      , proxies_(proxies)
    {}

    template<typename FieldType>
    void field(const char* name, const FieldType&)
    {
        if (!columns_.empty())
            columns_ += " ,";

        if (!bindings_.empty())
            bindings_ += " ,:";
        else bindings_ += ":";

        columns_ += name;
        bindings_ += name;

        proxies_.push_back(field_proxy<FieldType>());

        s_.exchange(use(*boost::any_cast<field_proxy<FieldType>>(&proxies_.back())));
    }

    void prepare_insert_statement(const char* table_name)
    {
        std::ostringstream stat_str;
        stat_str << "insert into " << table_name << " ( " << columns_ << " ) values ( " << bindings_ << " ) ";

        s_.define_and_bind();
        s_.prepare(stat_str.str());
    }

private:
    statement& s_;
    std::list<boost::any>& proxies_;
    std::string columns_;
    std::string bindings_;
};

struct bind_values
{
    bind_values(statement& s, std::list<boost::any>& proxies) 
        : s_(s), proxies_(proxies), iter_(proxies.begin()) {}

    template<typename FieldType>
    void field(const char*, const FieldType& val)
    {
        boost::any_cast<field_proxy<FieldType>>(&*iter_++)->ptr_ = &val;
    }

    statement& s_;
    std::list<boost::any>& proxies_;
    std::list<boost::any>::iterator iter_;
};

}}}
