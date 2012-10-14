#pragma once

#include <soci.h>

namespace soci { namespace orm { namespace detail {

/// \brief This proxy type helps to avoid SOCI rebinding problem.
/// http://www.mail-archive.com/soci-users@lists.sourceforge.net/msg00958.html
/// For each one-time prepared statements we going to have vector<field_proxy>.
/// We going to bind statement once to this proxies, and then we going to change pointer member
/// every time when we need to rebind statement
template<typename T>
struct field_proxy
{
    field_proxy() : ptr_(nullptr) {}
    const T* ptr_;
};

}} // namespace orm, detail

template<typename T> 
struct type_conversion<orm::detail::field_proxy<T>>
{
    typedef T base_type;

    static void from_base(const T& v, indicator /* ind */, orm::detail::field_proxy<T>& p)
    {
        //*p.ptr_ = v;
    }

    static void to_base(const orm::detail::field_proxy<T>& p, T& v, indicator & ind)
    {
        v = *p.ptr_;
    }
};

} // namespace soci,orm
