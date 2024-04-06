#ifndef PARTICLE_FORCE_GENERATOR_H
#define PARTICLE_FORCE_GENERATOR_H

#include<ffiseg/ffiseg.hpp>
#include<vector>

namespace ffiseg {
    class particle_force_generator {
    public:
        virtual void update_force(particle* part, real duration) = 0;
    };

    // registry keeps track of registrations.
    class particle_force_registry {
    public:
        void add(particle* part, particle_force_generator* fg);
        void remove(particle* part, particle_force_generator* fg);
        void clear();
        void update_forces(real duration);
    protected:
        struct particle_force_registration {
            particle* part;
            particle_force_generator* force_gen;
        };
        typedef std::vector<particle_force_registration> registry;
        registry registrations;
    };
}

#endif
