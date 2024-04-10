#include<ffiseg/particle_link.hpp>

using namespace ffiseg;

real particle_link::current_length() const {
   vector rel_pos = parts[0]->get_position() - parts[1]->get_position();
   return rel_pos.magnitude();
}

unsigned particle_cable::fill_contact(particle_contact* contact, unsigned limit) const {
    real length = current_length();
    if(length < max_length) return 0;

    contact->parts[0] = parts[0];
    contact->parts[1] = parts[1];

      // Calculate the normal.
    vector normal = parts[1]->get_position() - parts[0]->get_position();
    normal.normalize();
    contact->contact_normal = normal;
    contact->penetration = length - max_length;
    contact->restitution = restitution;
    return 1;
}
