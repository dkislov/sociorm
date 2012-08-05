#pragma once

#include <sociorm/detail/safe_bool.hpp>

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

    ptr(orm* orm, T&& c);

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
        soci::orm::orm* orm_;  //!< Parent ORM object.
        T obj_;                //!< Object itself.
        id_type id_;           //!< Object id in database.
        bool is_dirty_;        //!< True if user has called modify, mark that object has been modified and should be persisted.
    };

    std::shared_ptr<data> data_;
};

}} // namespace soci, orm
