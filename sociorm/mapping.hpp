#pragma once

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

}}
