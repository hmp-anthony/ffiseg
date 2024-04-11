#ifndef PARTICLE_FORCE_GENERATOR_H
#define PARTICLE_FORCE_GENERATOR_H

#include<ffiseg/particle.hpp>
#include<vector>

namespace ffiseg {
    // abstract base class / interface.
    class particle_force_generator {
    public:
        virtual void update_force(particle* part, real duration) = 0;
    };

    class particle_gravity : public particle_force_generator {
    public:
        particle_gravity(const vector& gravity);
        virtual void update_force(particle* part, real duration);
    private:
        vector gravity;
    };

    class particle_drag : public particle_force_generator {
    public:
        particle_drag(real k1, real k2);
        virtual void update_force(particle* part, real duration);
    private:
        real k1;
        real k2;
    };
   
    // generator that applies a spring force where one end is 
    // attached to a fixed point in space.
    class particle_anchored_spring : public particle_force_generator {
    public:
        particle_anchored_spring();
        particle_anchored_spring(vector* anchor, real spring_constant, real rest_length);
        const vector* get_anchor() const;
        void init(vector* anchor, real spring_constant, real rest_length);
        virtual void update_force(particle* part, real duration);
    protected:
        vector* anchor;
        real spring_constant;
        real rest_length;
    };

    class particle_anchored_bungee : public particle_anchored_spring {
    public:
        virtual void update_force(particle* part, real duration);
    };

    class particle_fake_spring : public particle_force_generator {
    public:
        particle_fake_spring(vector *anchor, real spring_constant, real damping);
        virtual void update_force(particle *part, real duration);
    private:
        vector* anchor;
        real spring_constant;
        real damping;

    };

    class particle_spring : public particle_force_generator {
    public:
        particle_spring(particle* other, real spring_constant, real rest_length);
        virtual void update_force(particle* part, real duration);
    private:
        particle* other;
        real spring_constant;
        real rest_length;
    };

    class particle_bungee : public particle_force_generator {
    public:
        particle_bungee(particle* other, real spring_constant, real rest_length);
        virtual void update_force(particle* part, real duration);
    private:
        particle* other;
        real spring_constant;
        real rest_length;
    };

    class particle_buoyancy : public particle_force_generator {
    public:
        particle_buoyancy(real max_depth, real volume, real water_height,
                          real liquid_density = 1000.0f);
        virtual void update_force(particle* part, real duration);
    private:
        real max_depth;
        real volume;
        real water_height;
        real liquid_density;
    };

    // registry keeps track of force registrations.
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
    public:
        // for testing purposes
        registry get_registrations();
    };
}

#endif
