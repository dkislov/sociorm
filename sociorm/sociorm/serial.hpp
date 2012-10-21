#pragma once

#include <type-conversion-traits.h>

namespace soci { namespace orm {

/// \brief Special type to distinguish database serial type.
/// 
/// If mapped type has key of serial type, then insert command will not have value and will 
/// completely rely on database to create it during insert.
/// It will also impact database schema creation and update. Depending on RDBMS it will use 
/// appropriate type or will additionaly create sequence generator.
class serial
{
public:
    typedef unsigned long long value_type;

    serial() : value_(value_type(-1)) {}
    serial(value_type value) : value_(value) {}

    operator value_type() const
    {
        return value_;
    }

    serial& operator=(value_type value)
    {
        value_ = value;
        return *this;
    }

private:
    value_type value_;
};

template<typename T>
struct is_serial : std::is_same<T, serial> {};

} // namespace orm

// Extend SOCI to support serial type
template<> 
struct type_conversion<orm::serial>
{
    typedef orm::serial::value_type base_type;

    static void from_base(const base_type& v, indicator /* ind */, orm::serial& p)
    {
        p = v;
    }

    static void to_base(const orm::serial& p, base_type& v, indicator & ind)
    {
        v = p;
    }
};

}
