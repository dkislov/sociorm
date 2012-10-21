#pragma once

#include <sociorm/orm.hpp>

namespace soci { namespace orm { namespace detail {

template<typename Class>
struct bind_values
{
    bind_values(manager& _orm, Class& obj, statement& s, std::list<field_proxy>& proxies) 
        : orm_(_orm), obj_(obj), s_(s), proxies_(proxies), iter_(proxies.begin()) {}

    void primary_key(const char*)
    {
        if (is_serial<typename Class::primary_key_type>::value)
            return;

        iter_++->set_pointer(&obj_.pk());
    }

    template<typename BaseType>
    void derived_from(const char*)
    {
        iter_++->set_value(orm_.save((BaseType&)obj_));
    }

    template<typename FieldType>
    void field(const char*, const FieldType& val)
    {
        iter_++->set_pointer(&val);
    }

    manager& orm_;
    Class& obj_;
    statement& s_;
    std::list<field_proxy>& proxies_;
    std::list<field_proxy>::iterator iter_;
};

}}} // namespace soci, orm, detail
