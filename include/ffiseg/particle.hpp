#ifndef PARTICLE_H
#define PARTICLE_H

#include<ffiseg/core.hpp>

namespace ffiseg {
class particle {
public:
    particle();
    particle(real x, real y, real z);
    void integrate(real duration);
    void set_mass(real m);
    real get_mass() const;
    void set_inverse_mass(real m);
    real get_inverse_mass() const;
    bool has_finite_mass() const;
    void set_damping(const real damping);
    real get_damping() const;

    //position
    void set_position(const vector& position);
    void set_position(const real x, const real y, const real z);
    void get_position(vector* position) const;
    vector get_position() const;

    //velocity
    void set_velocity(const vector& velocity);
    void set_velocity(const real x, const real y, const real z);
    void get_velocity(vector* velocity) const;
    vector get_velocity() const;

    //acceleration
    void set_acceleration(const vector& acceleration);
    void set_acceleration(const real x, const real y, const real z);
    void get_acceleration(vector* acceleration) const;
    vector get_acceleration() const;

    void clear_accumulator();
    void add_force(const vector& force);
protected:
    real inverse_mass;
    vector position;
    vector velocity;
    vector force_accumulation;
    vector acceleration;
    real damping;
};
}

#endif
