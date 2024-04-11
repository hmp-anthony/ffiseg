#include<ffiseg/particle_link.hpp>

using namespace ffiseg;

real particle_link::current_length() const {
   vector rel_pos = parts[0]->get_position() - parts[1]->get_position();
   return rel_pos.magnitude();
}

unsigned particle_cable::add_contact(particle_contact* contact, unsigned limit) const {
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

unsigned particle_rod::add_contact(particle_contact* contact, unsigned limit) const {
    real current_len = current_length();
    if(current_len == length) return 0;

    contact->parts[0] = parts[0];
    contact->parts[1] = parts[1];

      // Calculate the normal.
    vector normal = parts[1]->get_position() - parts[0]->get_position();
    normal.normalize();

    if (current_len > length) {
        contact->contact_normal = normal;
        contact->penetration = current_len - length;
    } else {
        contact->contact_normal = normal * -1;
        contact->penetration = length - current_len;
    }

    contact->restitution = 0;
    return 1;
}

real particle_constraint::current_length() const
{
    vector relative_pos = part->get_position() - anchor;
    return relative_pos.magnitude();
}

unsigned particle_cable_constraint::add_contact(particle_contact *contact,
                                   unsigned limit) const
{
    // Find the length of the cable
    real length = current_length();

    // Check if we're over-extended
    if (length < max_length)
    {
        return 0;
    }

    // Otherwise return the contact
    contact->parts[0] = part;
    contact->parts[1] = 0;

    // Calculate the normal
    vector normal = anchor - part->get_position();
    normal.normalize();
    contact->contact_normal = normal;

    contact->penetration = length - max_length;
    contact->restitution = restitution;

    return 1;
}

unsigned particle_rod_constraint::add_contact(particle_contact *contact,
                                 unsigned limit) const
{
    // Find the length of the rod
    real current_len = current_length();

    // Check if we're over-extended
    if (current_len == length)
    {
        return 0;
    }

    // Otherwise return the contact
    contact->parts[0] = part;
    contact->parts[1] = 0;

    // Calculate the normal
    vector normal = anchor - part->get_position();
    normal.normalize();

    // The contact normal depends on whether we're extending or compressing
    if (current_len > length) {
        contact->contact_normal = normal;
        contact->penetration = current_len - length;
    } else {
        contact->contact_normal = normal * -1;
        contact->penetration = length - current_len;
    }

    // Always use zero restitution (no bounciness)
    contact->restitution = 0;

    return 1;
}
