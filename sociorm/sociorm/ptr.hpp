#pragma once

#include <sociorm/detail/safe_bool.hpp>
#include <sociorm/detail/class_info.hpp>

namespace soci { namespace orm {

class orm;

/// \brief Maintain connectivity between single C++ object and it`s database representation
/// \todo Custom allocator support.
/// \warn NOT-thread safe.
template<typename T>
class ptr : detail::safe_bool< ptr<T> >
{
public:
    // TODO: Currently this is int, but on next step we must support complex id type within type traits.
    typedef int id_type;

    /// \brief Construct null pointer
    ptr();

    /// \brief Construct pointer from object, insert object into database
    ptr(soci::orm::orm* orm, T&& c, id_type id);

    /// \brief Construct pointer from object id, read object from database

    /// \brief Flush object into database, if marked dirty.
    void flush();

    /// \brief Mark object as dirty and return modifiable reference to object.
    T* modify();

    /// \brief Remove object from database.
    void remove();

    /// \brief Reread object from database.
    void reread();

    /// \brief Return pointer to const object.
    const T* operator->() const;
    /// \brief Return pointer to const object.
    const T* get() const;

    /// \brief Perform boolean test.
    bool boolean_test() const;

    /// \brief Return parent orm object.
    orm& orm();

    /// \brief Return object id
    id_type id();

    friend bool operator<(const ptr<T>& f, const ptr<T>& s);

private:
    struct data
    {
        data(soci::orm::orm* orm, T&& obj, id_type id) 
            : orm_(orm)
            , obj_(std::forward<T>(obj))
            , id_(id)
            , is_dirty_(false)
        {
        }

        soci::orm::orm* orm_;  //!< Parent ORM object.
        T obj_;                //!< Object itself.
        id_type id_;           //!< Object id in database.
        bool is_dirty_;        //!< True if user has called modify, mark that object has been modified and should be persisted.
    };

    std::shared_ptr<data> data_;
};

template<typename T>
ptr<T>::ptr()
{
}

template<typename T>
ptr<T>::ptr(soci::orm::orm* orm, T&& c, id_type id)
    : data_(std::make_shared<T>(orm, std::forward<T>(c), id))
{
}

template<typename T>
void ptr<T>::flush()
{
    BOOST_ASSERT(data_ && "null soci::orm::ptr object")
    // Get prepared flush query for type. 
    // Good place to put it is class_info
}

template<typename T>
T* ptr<T>::modify()
{
    data_->is_dirty_ = true;
    return &data_->obj_;
}

template<typename T>
void ptr<T>::remove()
{
    BOOST_ASSERT(data_ && "null soci::orm::ptr object")
    // Get prepared remove query for type.
}

template<typename T>
void ptr<T>::reread()
{
    BOOST_ASSERT(data_ && "null soci::orm::ptr object")
    // Get prepared select by id query for type.
}

template<typename T>
const T* ptr<T>::operator->() const
{
    BOOST_ASSERT(data_ && "null soci::orm::ptr object")
    return &data_->obj_;
}

/// \brief Return pointer to const object.
template<typename T>
const T* ptr<T>::get() const
{
    BOOST_ASSERT(data_ && "null soci::orm::ptr object")
    return &data_->obj_;
}

/// \brief Perform boolean test.
template<typename T>
bool ptr<T>::boolean_test() const
{
    return data_;
}

/// \brief Return parent orm object.
template<typename T>
orm& ptr<T>::orm()
{
    BOOST_ASSERT(data_ && "null soci::orm::ptr object")
}

/// \brief Return object id
template<typename T>
typename ptr<T>::id_type ptr<T>::id();

}} // namespace soci, orm
