#include<ffiseg/particle_force_generator.hpp>

using namespace ffiseg;

particle_gravity::particle_gravity(const vector& gravity) : gravity(gravity) {}

void particle_gravity::update_force(particle* part, real duration) {
    if (!part->has_finite_mass()) return;
    part->add_force(gravity * part->get_mass());
}

particle_drag::particle_drag(real k1, real k2) : k1(k1), k2(k2) {}

void particle_drag::update_force(particle* part, real duration) {
    vector force;
    part->get_velocity(&force);

    // calculate the total drag coefficient
    real drag_coeff = force.magnitude();
    drag_coeff = k1 * drag_coeff + k2 * drag_coeff * drag_coeff;

    // calculate the final force and apply it - see page 100
    force.normalize();
    force *= -drag_coeff;
    part->add_force(force);
}

particle_anchored_spring::particle_anchored_spring() {}

particle_anchored_spring::particle_anchored_spring(vector *anchor, real sc, real rl) 
    : anchor(anchor), spring_constant(sc), rest_length(rl) {}

const vector* particle_anchored_spring::get_anchor() const { 
    return anchor;
}

void particle_anchored_spring::init(vector* anchor, real sc, real rl) {
    particle_anchored_spring::anchor = anchor;
    particle_anchored_spring::spring_constant = sc;
    particle_anchored_spring::rest_length = rl;
}

void particle_anchored_spring::update_force(particle* part, real duration)
{
    // Calculate the vector of the spring
    vector force;
    part->get_position(&force);
    force -= *anchor;

    // Calculate the magnitude of the force
    real magnitude = force.magnitude();
    magnitude = (rest_length - magnitude) * spring_constant;

    // Calculate the final force and apply it
    force.normalize();
    force *= magnitude;
    part->add_force(force);
}

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
