#include<cstddef>
#include<ffiseg/particle_world.hpp>

#include<iostream>

using namespace ffiseg;

particle_world::particle_world(unsigned max_contacts, unsigned iterations)
                             : resolver(iterations), max_contacts(max_contacts) {
    part_contacts = new particle_contact[max_contacts];
    calculate_iterations = (iterations == 0);
}

particle_world::~particle_world() {
    delete[] part_contacts;
}

unsigned particle_world::generate_contacts() {
    unsigned limit = max_contacts;
    particle_contact* next_contact = part_contacts;
    for(std::vector<particle_contact_generator*>::iterator g = contact_generators.begin();
        g != contact_generators.end();
        g++) {
        unsigned used = (*g)->add_contact(next_contact, limit);
        std::cout << used << std::endl;
        limit -= used;
        next_contact += used;

        if(limit <= 0) break;
    }
    return max_contacts - limit;
}

void particle_world::integrate(real duration) {
    for(std::vector<particle*>::iterator p = parts.begin();
        p != parts.end();
        p++) {
        (*p)->integrate(duration);
    }
}

void particle_world::run_physics(real duration) {
    force_registry.update_forces(duration);
    integrate(duration);
    unsigned used_contacts = generate_contacts();
    if(used_contacts) {
        if (calculate_iterations) resolver.set_iterations(used_contacts * 2);
        resolver.resolve_contacts(part_contacts, used_contacts, duration);
    }
}

void particle_world::start_frame() {
    for(std::vector<particle*>::iterator p = parts.begin();
        p != parts.end();
        p++) {
        (*p)->clear_accumulator();
    }
}

std::vector<particle*>& particle_world::get_particles()
{
    return parts;
}

std::vector<particle_contact_generator*>& particle_world::get_contact_generators()
{
    return contact_generators;
}

particle_force_registry& particle_world::get_force_registry()
{
    return force_registry;
}

void ground_contacts::init(std::vector<particle*>* particles)
{
    parts = particles;
}

unsigned ground_contacts::add_contact(particle_contact *contact, unsigned limit) const
{
    std::cout << "aaaaa" << std::endl;
    unsigned count = 0;
    for (std::vector<particle*>::iterator p = parts->begin();
        p != parts->end();
        p++) {

        real y = (*p)->get_position().get_y();
        if (y < 0.0f)
        {
            contact->contact_normal = ffiseg::vector(0,1,0);
            contact->parts[0] = *p;
            contact->parts[1] = NULL;
            contact->penetration = -y;
            contact->restitution = 0.2f;
            contact++;
            count++;
        }

        if (count >= limit) return count;
    }
    std::cout << "bbbbaaaaa" << std::endl;
    return count;
}
