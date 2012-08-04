#pragma once

namespace soci { namespace orm {

template<typename Class>
class ptr
{
    const Class* operator->() const;
    Class* modify();
    void remove();
};

}}
