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

void particle_damper::update_force(particle* part, real duration) {
    vector force;
    part->get_velocity(&force);
    force -= other->get_velocity();
    force *= -1 * k;
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
    vector force;
    part->get_position(&force);
    force -= *anchor;

    real magnitude = force.magnitude();
    magnitude = (rest_length - magnitude) * spring_constant;

    force.normalize();
    force *= magnitude;
    part->add_force(force);
}

void particle_anchored_bungee::update_force(particle* part, real duration) {
    vector force;
    part->get_position(&force);
    force -= *anchor;

    real magnitude = force.magnitude();
    if (magnitude < rest_length) return;

    magnitude = magnitude - rest_length;
    magnitude *= spring_constant;

    force.normalize();
    force *= -magnitude;
    part->add_force(force);
}

particle_fake_spring::particle_fake_spring(vector *anchor, real sc, real d)
                     : anchor(anchor), spring_constant(sc), damping(d) {}

void particle_fake_spring::update_force(particle* part, real duration)
{
    // Check that we do not have infinite mass
    if (!part->has_finite_mass()) return;

    // Calculate the relative position of the particle to the anchor
    vector position;
    part->get_position(&position);
    position -= *anchor;

    // Calculate the constants and check they are in bounds.
    real gamma = 0.5f * real_sqrt(4 * spring_constant - damping*damping);
    if (gamma == 0.0f) return;
    vector c = position * (damping / (2.0f * gamma)) +
        part->get_velocity() * (1.0f / gamma);

    // Calculate the target position
    vector target = position * real_cos(gamma * duration) +
        c * real_sin(gamma * duration);
    target *= real_exp(-0.5f * duration * damping);

    // Calculate the resulting acceleration and therefore the force
    vector accel = (target - position) * ((real)1.0 / (duration*duration)) -
        part->get_velocity() * ((real)1.0/duration);
    part->add_force(accel * part->get_mass());
}

particle_spring::particle_spring(particle *other, real sc, real rl)
                               : other(other), spring_constant(sc), rest_length(rl) {}

void particle_spring::update_force(particle* part, real duration)
{
    // Calculate the vector of the spring
    vector force;
    part->get_position(&force);
    force -= other->get_position();

    // Calculate the magnitude of the force
    real magnitude = force.magnitude();
    magnitude = real_abs(magnitude - rest_length);
    magnitude *= spring_constant;

    // Calculate the final force and apply it
    force.normalize();
    force *= -magnitude;
    part->add_force(force);
}

particle_pseudo_spring::particle_pseudo_spring(particle *other, real sc, real rl)
                               : other(other), spring_constant(sc), rest_length(rl) {}

void particle_pseudo_spring::update_force(particle* part, real duration)
{
    // Calculate the vector of the spring
    vector force;
    part->get_position(&force);
    force -= other->get_position();

    // Calculate the magnitude of the force
    real magnitude = force.magnitude();
    magnitude = magnitude - rest_length;
    magnitude *= spring_constant;

    // Calculate the final force and apply it
    force.normalize();
    force *= -magnitude;
    part->add_force(force);
}

particle_bungee::particle_bungee(particle *other, real sc, real rl)
                               : other(other), spring_constant(sc), rest_length(rl) {}

void particle_bungee::update_force(particle* part, real duration)
{
    // Calculate the vector of the spring
    vector force;
    part->get_position(&force);
    force -= other->get_position();

    // Check if the bungee is compressed
    real magnitude = force.magnitude();
    if (magnitude <= rest_length) return;

    // Calculate the magnitude of the force
    magnitude = spring_constant * (rest_length - magnitude);

    // Calculate the final force and apply it
    force.normalize();
    force *= -magnitude;
    part->add_force(force);
}


particle_buoyancy::particle_buoyancy(real max_depth,
                                     real volume,
                                     real water_height,
                                     real liquid_density)
: max_depth(max_depth), volume(volume), water_height(water_height), liquid_density(liquid_density) {}

void particle_buoyancy::update_force(particle* part, real duration)
{
    real depth = part->get_position().get_y();

    if (depth >= water_height + max_depth) return;
    vector force(0,0,0);

    if (depth <= water_height - max_depth) {
        force.set_y(liquid_density * volume);
        part->add_force(force);
        return;
    }

    force.set_y(liquid_density * volume * (depth - max_depth - water_height) / (2 * max_depth));
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

particle_force_registry::registry particle_force_registry::get_registrations() {
    return registrations;
}
