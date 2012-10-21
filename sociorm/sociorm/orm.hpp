#pragma once

#include <soci.h>

#include <sociorm/base.hpp>
#include <sociorm/ptr.hpp>
#include <sociorm/query.hpp>
#include <sociorm/detail/field_proxy.hpp>
#include <sociorm/detail/type_info_ptr_less.hpp>

#include <string>
#include <memory>

namespace soci { namespace orm {

/// Primary key selection:
/// Persist must contain one of 
/// - primary_key("id", id_)
/// - base<base_type>()

class manager
{
public:
    typedef std::map<const std::type_info*, detail::class_info, detail::type_info_ptr_less> class_info_map;

    manager(soci::session& s);
    manager(backend_factory const & factory, std::string const & connect_string);
    manager(std::string const & backendName, std::string const & connect_string);
    explicit manager(std::string const & connectString);
    explicit manager(connection_pool & pool);

    /// \brief Return underlying soci session.
    session& session();

    /// \brief Setup mapping between class and database table
    template<typename Class>
    void map_class(const char* table_name);

    /// \brief Create or update database schema
    template<typename Class>
    void update_schema();

    /// \brief Insert new object to database, and return primary key
    template<typename Class>
    typename std::remove_reference<Class>::type::primary_key_type save(Class&& obj);

    /// \brief Insert new object to database and return reference to created object.
    template<typename Class>
    ptr<Class> add_getptr(Class&& obj);

    /// \brief Find new object in database.
    ///
    /// Return proxy object that can be implicitly converted to ptr<Class> or collection<ptr<Class>>. 
    /// Implicit conversion make proxy to invoke sql query.
    ///
    /// \code
    ///
    /// // Query first object that satisfies conditions
    /// ptr<User> joe = manager.query<User>().where("name = ?").bind("Joe");
    ///
    /// // The same as above
    /// ptr<User> joe = manager.query<ptr<User>>().where("name = ?").bind("Joe");
    /// 
    /// // Query all objects satisfying conditions, iterate over them
    /// typedef collection<ptr<User>> Users;
    /// 
    /// Users users = session.query<User>();
    /// 
    /// std::cerr << "We have " << users.size() << " users:" << std::endl;
    /// 
    /// for(ptr<User> user : users) // c++11 range based for
    /// {
    ///     std::cerr << " user " << user->name
    ///	              << " with karma of " << user->karma << std::endl;
    /// }
    /// 
    /// // Make non object query
    /// int count = manager.query<int>("select count(1) from user").where("name = ?").bind("Joe");
    ///
    /// \endcode
    template<typename Class>
    query<Class> query();

private:
    struct smart_deleter 
    {
        smart_deleter(bool del) : del_(del) {}

        void operator()(soci::session* s)
        {
            if (del_) 
                delete s;
        }

    private:
        bool del_;
    };

    std::unique_ptr<soci::session, smart_deleter> session_;
    class_info_map class_data_;
};

}} // namespace soci, orm

#include <sociorm/detail/collect_names_and_exchange_action.hpp>
#include <sociorm/detail/bind_values_action.hpp>

namespace soci { namespace orm {

inline manager::manager(soci::session& s)
    : session_(&s, smart_deleter(false))
{    
}

inline manager::manager(backend_factory const & factory, std::string const & connect_string)
    : session_(new soci::session(factory, connect_string), smart_deleter(true))
{
}

inline manager::manager(std::string const & backend_name, std::string const & connect_string)
    : session_(new soci::session(backend_name, connect_string), smart_deleter(true))
{
}

inline manager::manager(std::string const & connect_string)
    : session_(new soci::session(connect_string), smart_deleter(true))
{
}

inline manager::manager(connection_pool & pool)
    : session_(new soci::session(pool), smart_deleter(true))
{
}

inline session& manager::session()
{
    return *session_;
}

template<typename Class>
void manager::map_class(const char* table_name)
{
    detail::class_info& data = class_data_[&typeid(Class)];

    if (data.insert_)
        return;

    data.table_ = table_name;

    data.insert_.reset(new statement(session()));
    data.insert_->alloc();

    detail::collect_names_and_exchange_action<Class> collect_action(this->class_data_, *data.insert_, data.insert_use_proxies_);

    Class& obj = *(Class*)nullptr;
    obj.persist(collect_action);

    collect_action.prepare_insert_statement(table_name);
}

template<typename Class>
typename std::remove_reference<Class>::type::primary_key_type manager::save(Class&& obj)
{
    typedef typename std::remove_reference<Class>::type class_type;

    detail::class_info& data = class_data_.at(&typeid(class_type));

    detail::bind_values<class_type> bind_action(*this, obj, *data.insert_, data.insert_use_proxies_);
    obj.persist(bind_action);

    data.insert_->execute(true);

    if (is_serial<class_type::primary_key_type>::value)
    {
        long id;
        session().get_last_insert_id(data.table_, id);
        return id;
    }
    else return obj.pk();
}

/// \brief Override this to make type persistable non-intrusively.
template<typename Action, typename Type>
void persist(Action& a, Type& obj)
{
	obj.persist(a);
}

}}

