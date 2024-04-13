#ifndef PARTICLE_LINK_H
#define PARTICLE_LINK_H

#include<ffiseg/particle_contact.hpp>

namespace ffiseg {
    class particle_link : public particle_contact_generator {
    public:
    // see page 149
        particle* parts[2];
        virtual unsigned add_contact(particle_contact* contact, unsigned limit) const = 0;
    protected:
        real current_length() const;
    };

    class particle_cable : public particle_link {
    public:
        virtual unsigned add_contact(particle_contact* contact, unsigned limit) const;
        real max_length;
        real restitution;
    };

    class particle_rod : public particle_link {
    public:
        particle_rod() {length = 0;}
        real length;
        virtual unsigned add_contact(particle_contact* contact, unsigned limit) const;
    };

    class particle_constraint : public particle_contact_generator {
    public:
        particle* part;
        vector anchor;
        virtual unsigned add_contact(particle_contact* contact, unsigned limit) const = 0;
    protected:
        real current_length() const;
    };

    class particle_cable_constraint : public particle_constraint {
    public:
        real max_length;
        real restitution;
        virtual unsigned add_contact(particle_contact* contact, unsigned limit) const;
    };

    class particle_rod_constraint : public particle_constraint {
    public:
        real length;
        virtual unsigned add_contact(particle_contact* contact, unsigned limit) const;
    };
}
#endif
