#pragma once

#include <type_traits>

namespace soci { namespace orm {

namespace detail {

/// \brief Non-template base type for base class.
struct base_iface
{
    virtual ~base_iface() = 0 {} 
};

} // namespace detail

/// \brief Base type for all user type that wants to participate in object relational mapping.
///
/// Also acts as holder of primary key value. The reason is to have common primary key across 
/// class hierarchy.
template<typename PrimaryKeyType>
class base : detail::base_iface
{
public:
    typedef PrimaryKeyType primary_key_type;

    virtual ~base() {}

    primary_key_type& pk()
    {
        return key_;
    }

    const primary_key_type& pk() const
    {
        return key_;
    }

    void pk(const primary_key_type& key)
    {
        key_ = key;
    }

private:
    primary_key_type key_;
};

} // namespace orm

/// We have common type conversion for all types derived from orm::base.
/// This specialization redirects field mapping to T::persist method
template<typename T> 
struct type_conversion<
    T
  , typename std::enable_if<std::is_base_of<orm::detail::base_iface, T>::value>::type
  >
{
    typedef values base_type;

    struct from_base_action
    {
        from_base_action(const values& v) : values_(v) {}

        void primary_key(const char* name)
        {
        }

        template<typename BaseType>
        void derived_from(const char* name)
        {
        }

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

        void primary_key(const char* name)
        {
        }

        template<typename BaseType>
        void derived_from(const char* name)
        {
        }

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