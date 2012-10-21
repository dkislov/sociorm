#pragma once

#include <soci.h>

#include <boost/any.hpp>
#include <boost/variant.hpp>

namespace soci { namespace orm { namespace detail {

/// \brief This proxy type helps to avoid SOCI rebinding problem.
/// http://www.mail-archive.com/soci-users@lists.sourceforge.net/msg00958.html
/// For each one-time prepared statements we going to have vector<field_proxy>.
/// We going to bind statement once to this proxies, and then we going to change pointer member
/// every time when we need to rebind statement
class field_proxy
{
public:

    template<typename T>
    static field_proxy create()
    {
        typedef boost::variant<const T*, T> value_type;
        field_proxy p;
        p.value_ = value_type();
        return p;
    }

    template<typename T>
    void set_pointer(const T* ptr)
    {
        get<T>() = ptr;
    }

    template<typename T>
    void set_value(const T& val)
    {
        get<T>() = val;
    }

    template<typename T>
    boost::variant<const T*, T>& get()
    {
        return *boost::any_cast<boost::variant<const T*, T>>(&value_);
    }

    template<typename T>
    const boost::variant<const T*, T>& get() const
    {
        return *boost::any_cast<boost::variant<const T*, T>>(&value_);
    }

private:
    field_proxy() {}

    boost::any value_;
};

}} // namespace orm, detail

// Enable soci to use variant from field_proxy
template<typename T> 
struct type_conversion<
    boost::variant<const T*, T>
//  , typename std::enable_if<std::is_base_of<orm::detail::base_iface, T>::value>::type
  >
{
    typedef T base_type;
    typedef boost::variant<const T*, T> variant_type;

    static void from_base(const T& v, indicator /* ind */, variant_type& p)
    {
        // TODO: Generalize to non-const version
    }

    static void to_base(const variant_type& p, T& v, indicator & ind)
    {
        struct visitor : boost::static_visitor<>
        {
            visitor(T& v) : v_(v) {}

            void operator()(const T& obj) 
            {
                v_ = obj;
            }

            void operator()(const T* obj)
            {
                v_ = *obj;
            }

            T& v_;
        } vis(v);

        p.apply_visitor(vis);
    }
};

} // namespace soci
