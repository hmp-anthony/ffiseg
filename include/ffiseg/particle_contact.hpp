#ifndef PARTICLE_CONTACT_H
#define PARTICLE_CONTACT_H

#include<ffiseg/particle.hpp>

#include<vector>

namespace ffiseg {

    class particle_contact_resolver;

    class particle_contact {
        friend class particle_contact_resolver;
    // replace with getters and setters
    public:
        particle* parts[2];
        real restitution;
        vector contact_normal;
        real penetration;
        vector particle_movement[2];
    protected:
        void resolve(real duration);
        real calculate_separating_velocity() const;
    private:
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
