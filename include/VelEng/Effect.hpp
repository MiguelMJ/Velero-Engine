#ifndef _VELERO_EFFECT_HPP_
#define _VELERO_EFFECT_HPP_

namespace ven{
    template <class T, class U>
    class Effect{
    public:
        U info;
        void (*fun)(T&, const U&);
    };
}

#endif
