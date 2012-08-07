#pragma once


#include <sociorm/detail/class_info.hpp>
#include <soci.h>

namespace soci { namespace orm { namespace detail {

class orm_backend 
{
public:
    /// \brief Update class 
    virtual void update_table(soci::session& session, const class_info& ci) = 0;

    virtual ~orm_backend() {}
};

}}}
