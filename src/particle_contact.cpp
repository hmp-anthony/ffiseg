#include<ffiseg/particle_contact.hpp>

void particle_contact::resolve(real duration) {
    resolve_velocity(duration);
    resolve_interpenetration(duration);
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

    // check the velocity build-up due to acceleration only.
    vector acc_caused_velocity = parts[0]->get_acceleration();
    if(part[1]) {
        acc_caused_velocity -= parts[1]->get_acceleration();
    }

    real acc_caused_sep_velocity = acc_caused_velocity * contact_normal * duration;
    
    // if we have got a closing veclocity due to acceleration build up
    // remove it from the sep vel.
    if(acc_caused_sep_velocity < 0) {
        new_sv += restitution * acc_caused_sep_velocity;
        if(new_sv < 0) new_sv = 0;
    }

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
    // page 131; p' = p + g/m; g - impulse
    parts[0]->set_velocity(parts[0]->get_velocity()
    + impulse_per_inverse_mass * parts[0]->get_inverse_mass());
    
    if(parts[1]) {
        parts[1]->set_velocity(parts[1]->get_velocity()
        + impulse_per_inverse_mass * -parts[1]->get_inverse_mass());
    }
}

void particle_contact::resolve_interpenetration(real duration) {
    if(penetration <= 0) return;
    
    real total_inverse_mass = parts[0]->get_inverse_mass();
    if(parts[1]) {
        total_inverse_mass += parts[1]->get_inverse_mass();
    }

    if(total_inverse_mass <= 0) return;
   
    vector move_per_inverse_mass = contact_normal * (penetration / total_inverse_mass);

    particle_movement[0] = move_per_inverse_mass * parts[0]->get_inverse_mass();
    if(parts[1]) {
        particle_movement[1] = move_per_inverse_mass * -parts[1]->get_inverse_mass();
    } else {
        particle_movement[1].clear();
    }

    parts[0]->set_position(parts[0]->get_position() + particle_movement[0]);
    if(parts[1]) {
        parts[1]->set_position(parts[1]->get_position() + particle_movement[1]);
    }
}

particle_contact_resolver::particle_contact_resolver(unsigned iters) : iterations(iters) {}

void particle_contact_resolver::set_iterations(unsigned iters) {
    iterations = iters;
}

void particle_contact_resolver::resolve_contacts(particle_contact* contact_array,
                                                 unsigned contact_array_size,
                                                 real duration) {
    unsigned i = 0;
    iterations_used = 0;

    while(iterations_used < iterations) {
        real max = REAL_MAX;
        unsigned max_index = contact_array_size;

        for(i = 0; i < contact_array_size; ++i) {
            real sv = contact_array[i].calculate_separating_velocity();
            if(sv < max && (sv < 0 || contact_array[i].penetration > 0)) {
                max = sv;
                max_index = i;
            }
        }
        
        if(max_index == contact_array_size) break;

        contact_array[max_index].resolve(duration);
        vector* move = contact_array[max_index].particle_movement;

        for (i = 0; i < contact_array_size; i++) {
            if (contact_array[i].parts[0] == contact_array[max_index].parts[0])
            {
                contact_array[i].penetration -= move[0] * contact_array[i].contact_normal;
            }

            // isnt this a bug?! BUG
            else if (contact_array[i].parts[0] == contact_array[max_index].parts[1])
            {
                contact_array[i].penetration -= move[1] * contact_array[i].contact_normal;
            }
            if (contact_array[i].parts[1])
            {
                if (contact_array[i].parts[1] == contact_array[max_index].parts[0])
                {
                    contact_array[i].penetration += move[0] * contact_array[i].contact_normal;
                }
                else if (contact_array[i].parts[1] == contact_array[max_index].parts[1])
                {
                    contact_array[i].penetration += move[1] * contact_array[i].contact_normal;
                }
            }
        }

        iterations_used++
    }
}
