#ifndef PARTICLE_LINK_H
#define PARTICLE_LINK_H

#include<ffiseg/particle_contact.hpp>

namespace ffiseg {
    class particle_link {
    public:
    // see page 149
        virtual unsigned fill_contact(particle_contact* contact, unsigned limit) const = 0;
    protected:
        real current_length() const;
        particle* parts[2];
    };

    class particle_cable : public particle_link {
    public:
        virtual unsigned fill_contact(particle_contact* contact, unsigned limit) const;
    private:
        real max_length;
        real restitution;
    };

}
#endif
