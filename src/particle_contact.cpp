#include<ffiseg/particle_contact.hpp>

void particle_contact::resolve(real duration) {
    resolve_velocity(duration);
}

real particle_contact::calculate_separating_velocity() const {
    vector relative_velocity = parts[0]->get_velocity();
    if(parts[1]) {
        relative_velocity -= parts[1]->get_velocity();
    }
    return relative_velocity * contact_normal;
}

void particle_contact::resolve_velocity(real duration) {
    real sv = calculate_separating_velocity();

    // if moving away from each other --> velocity has been resolved.
    if(sv > 0) {
        // we need do nothing.
        return;
    }
    
    real new_sv = -sv * restitution;
    real delta_velocity = new_sv - sv;
    
    // objects with larger mass get less change in velocity.
    real total_inverse_mass = parts[0]->get_inverse_mass();
    if(parts[1]) {
        total_inverse_mass += parts[1]->get_inverse_mass();
    }
    
    if(total_inverse_mass <= 0) return;
    real impulse = delta_velocity / total_inverse_mass;
    vector impulse_per_inverse_mass = contact_normal * impulse;

    // apply impulses;
    parts[0]->set_velocity(parts[0]->get_velocity()
    + impulse_per_inverse_mass * parts[0]->get_inverse_mass());
    
    if(parts[1]) {
        parts[1]->set_velocity(parts[1]->get_velocity()
        + impulse_per_inverse_mass * -parts[1]->get_inverse_mass());
    }
}
