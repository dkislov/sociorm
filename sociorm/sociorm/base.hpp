#pragma once

#include <type_traits>

namespace soci { namespace orm {

/// \brief Base type for all user type that wamts to participate in object relational mapping
struct base
{
};

} //namespace orm

/// We have common type conversion for all types derived from orm::base.
/// This specialization redirects field mapping to T::persist method
template<typename T> 
struct type_conversion<
    T
  , typename std::enable_if<std::is_base_of<orm::base, T>::value>::type
  >
{
    typedef values base_type;

    struct from_base_action
    {
        from_base_action(const values& v) : values_(v) {}

        template<typename FieldType>
        void field(const char* name, FieldType& value)
        {
            value = values_.get<FieldType>(name);
        }

    private:
        const values& values_;
    };

    struct to_base_action
    {
        to_base_action(values& v) : values_(v) {}

        template<typename FieldType>
        void field(const char* name, FieldType& value)
        {
            v.set(name, value);
        }
    };

    static void from_base(const values& v, indicator /* ind */, T& p)
    {
        from_base_action action(v);
        p.persist(action);
    }

    static void to_base(const T& p, values& v, indicator & ind)
    {
        to_base_action action(v);
        p.persist(action);
    }
};


}