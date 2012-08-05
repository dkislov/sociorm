#pragma once

namespace soci { namespace orm {

template<typename T> class collection;

// TODO: Classify Result type, if this is ptr<T>, or non class type. Special handling for this cases
// TODO: Figure out is it copy or move constructible.
template<typename Result>
class query
{
public:
    /// \brief Bind value to next position marker
    template<typename T>
    void bind(const T& value);

    operator Result() const;
    operator collection<ptr<Result>>() const;

    query& where(const char* cond);
    query& order_by(const char* field);
    query& group_by(const char* fields);
    query& offset(int count);
    int offset() const;
    query& limit(int count);
    int limit() const;
};

}}
