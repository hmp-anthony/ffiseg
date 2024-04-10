#ifndef PARTICLE_CONTACT_H
#define PARTICLE_CONTACT_H

#include<ffiseg/particle.hpp>

#include<vector>

namespace cyclone {

    class particle_contact_resolver;

    class particle_contact {
        friend class particle_contact_resolver;
    protected:
        void resolve(real duration);
        real calculate_separating_velocity() const;
    private:
        particle* parts[2];
        real restitution;
        vector contact_normal;
        real penetration;
        vector particle_movement[2];

        void resolve_velocity(real duration);
        void resolve_interpenetration(real duration);
    };

    class particle_contact_resolver {
    public:
        particle_contact_resolver(unsigned inters);
        void set_iterations(unsigned iters);
        void resolve_contacts(particle_contact* contact_array,
                              unsigned contact_array_size,
                              real duration);
    protected:
        unsigned iterations;
        unsigned iterations_used;
    };
}

#endif
