#include<ffiseg/particle_force_generator.hpp>

using namespace ffiseg;

void particle_force_registry::add(particle* part, 
                                  particle_force_generator* force_gen) {
    particle_force_registry::particle_force_registration reg;
    reg.part = part;
    reg.force_gen = force_gen;
    registrations.push_back(reg);
}

void particle_force_registry::remove(particle* part,
                                     particle_force_generator* force_gen) {
    registry::iterator it = registrations.begin();
    for (; it != registrations.end(); it++) {
        if(it->part == part && it->force_gen == force_gen) {
            registrations.erase(it);
        }
    }
}

void particle_force_registry::clear() {
    registrations.clear();
}

void particle_force_registry::update_forces(real duration) {
    registry::iterator it = registrations.begin();
    for (; it != registrations.end(); it++) {
        it->force_gen->update_force(it->part, duration);
    }
}
