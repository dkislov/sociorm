#pragma once

#include <sociorm/detail/class_info.hpp>

namespace soci { namespace orm {

/// \brief Define mapping for simple field.
template<typename Action, typename Value>
void field(Action& a, Value& value, const char* column_name);

/// \brief Define one-to-many relation or many-to-many relations
template<typename Action, typename Value>
void has_many(Action& a, collection<ptr<Value>>& value, const char* foreing_table_name);

/// \brief Define one-to-many relation or many-to-many relations
template<typename Action, typename Value>
void belongs_to(Action& a, ptr<Value> value, const char* fk_column_name);

/// \brief Define hierarchy relation.
template<typename Action, typename BaseType>
void base_type(Action& a);

template<typename Value>
void field(detail::class_info& ci, Value&, const char* column_name)
{
    field_info fi = {

    ci.fields_.push_back(
}

}}
