#ifndef PARTICLE_WORLD_H
#define PARTICLE_WORLD_H

#include<ffiseg/particle_force_generator.hpp>
#include<ffiseg/particle_link.hpp>

namespace ffiseg {
    class particle_world {
    public:
        particle_world(unsigned max_contacts, unsigned iterations = 0);
        ~particle_world();
        unsigned generate_contacts();  // should this be protected?
        void integrate(real duration); // should this be protected?
        void run_physics(real duration);
        void start_frame();
        std::vector<particle*>& get_particles();
        std::vector<particle_contact_generator*>& get_contact_generators();
        particle_force_registry& get_force_registry();
        
    protected:
        std::vector<particle*>                      parts;
        bool                                        calculate_iterations;
        particle_force_registry                     force_registry;
        particle_contact_resolver                   resolver;
        std::vector<particle_contact_generator*>    contact_generators; 
        particle_contact*                           part_contacts;
        unsigned                                    max_contacts;
    };

    class ground_contacts : public particle_contact_generator {
    public:
        void init(std::vector<particle*>* parts);
        virtual unsigned addContact(particle_contact* contact, unsigned limit) const;
    private:
        std::vector<particle*>* parts;
    };
}

#endif
