#pragma once

#include <sociorm/orm.hpp>
#include <sociorm/detail/field_proxy.hpp>

#include <cassert>
#include <list>

namespace soci { namespace orm { namespace detail {

template<typename Class>
struct collect_names_and_exchange_action
{
    collect_names_and_exchange_action(
        manager::class_info_map& ci_map
      , statement& s
      , std::list<field_proxy>& proxies
      ) 
      : ci_map_(ci_map)
      , s_(s) 
      , proxies_(proxies)
    {}

    void primary_key(const char* name)
    {
        if (is_serial<typename Class::primary_key_type>::value)
            return;

        register_name<typename Class::primary_key_type>(name);
    }

    template<typename BaseType>
    void derived_from(const char* name)
    {
        class_info& self_type_ci = ci_map_[&typeid(Class)];
        class_info& base_type_ci = ci_map_[&typeid(BaseType)];

        assert(self_type_ci.base_type_ == nullptr && "Only one base can exists for type");
        self_type_ci.base_type_ = &base_type_ci;

        assert(
            std::find(base_type_ci.subtypes_.begin(), base_type_ci.subtypes_.end(), &self_type_ci) == base_type_ci.subtypes_.end() &&
            "Class can be registered as subtype for base class only once");
        base_type_ci.subtypes_.push_back(&self_type_ci);

        register_name<typename Class::primary_key_type>(name);
    }

    template<typename FieldType>
    void field(const char* name, const FieldType&)
    {
        register_name<FieldType>(name);
    }

    void prepare_insert_statement(const char* table_name)
    {
        std::ostringstream stat_str;
        stat_str << "insert into " << table_name << " ( " << columns_ << " ) values ( " << bindings_ << " ) ";

        s_.define_and_bind();
        s_.prepare(stat_str.str());
    }

private:
    template<typename T>
    void register_name(const char* name)
    {
        if (!columns_.empty())
            columns_ += " ,";

        if (!bindings_.empty())
            bindings_ += " ,:";
        else bindings_ += ":";

        columns_ += name;
        bindings_ += name;

        proxies_.push_back(field_proxy::create<T>());
        s_.exchange(use(proxies_.back().get<T>()));
    }

    manager::class_info_map& ci_map_;
    statement& s_;
    std::list<field_proxy>& proxies_;
    std::string columns_;
    std::string bindings_;
};

}}} // namespace soci, orm, detail
