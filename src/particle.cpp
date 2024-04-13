#include <assert.h>
#include <ffiseg/particle.hpp>

using namespace ffiseg;

particle::particle() {
    inverse_mass = 0;
    immune_from_physics = false;
}

particle::particle(real x, real y, real z) {
    inverse_mass = 0;
    position = vector(x, y, z);
    velocity = vector(0.0, 0.0, 0.0);
    damping = 1.0;
    immune_from_physics = false;
}

void particle::integrate(real duration) {
    if(immune_from_physics) return;
    if(inverse_mass <= 0.0f) return;
    assert(duration > 0.0);

    position.add_scaled_vector(velocity, duration);

    vector resulting_acceleration = acceleration;
    resulting_acceleration.add_scaled_vector(force_accumulation, inverse_mass);

    velocity.add_scaled_vector(resulting_acceleration, duration);

    velocity *= real_pow(damping, duration);

    clear_accumulator();
}

void particle::set_mass(real m) {
    assert(m != 0);
    particle::inverse_mass = 1.0/m; 
}

real particle::get_mass() const {
    if(inverse_mass == 0) {
        return REAL_MAX;
    } else {
        return 1.0/inverse_mass;
    }
}

void particle::set_inverse_mass(real im) {
    particle::inverse_mass = im;
}

real particle::get_inverse_mass() const {
    return inverse_mass;
}

bool particle::has_finite_mass() const {
    return inverse_mass >= 0.0f;
}

void particle::set_damping(const real damping) {
    particle::damping = damping;
}

real particle::get_damping() const {
    return damping;
}

void particle::set_position(const vector &position)
{
    particle::position = position;
}

void particle::set_position(const real x, const real y, const real z)
{
    position.set_x(x);
    position.set_y(y);
    position.set_z(z);
}

void particle::get_position(vector *position) const
{
    *position = particle::position;
}

vector particle::get_position() const
{
    return position;
}

void particle::set_velocity(const vector &velocity)
{
    particle::velocity = velocity;
}

void particle::set_velocity(const real x, const real y, const real z)
{
    velocity.set_x(x);
    velocity.set_y(y);
    velocity.set_z(z);
}

void particle::get_velocity(vector *velocity) const
{
    *velocity = particle::velocity;
}

vector particle::get_velocity() const
{
    return velocity;
}

void particle::set_acceleration(const vector &acceleration)
{
    particle::acceleration = acceleration;
}

void particle::set_acceleration(const real x, const real y, const real z)
{
    acceleration.set_x(x);
    acceleration.set_y(y);
    acceleration.set_z(z);
}

void particle::get_acceleration(vector *acceleration) const
{
    *acceleration = particle::acceleration;
}

vector particle::get_acceleration() const
{
    return acceleration;
}

void particle::clear_accumulator()
{
    force_accumulation.clear();
}

void particle::add_force(const vector &force)
{
    force_accumulation += force;
}

void particle::make_immune_from_physics() {
    immune_from_physics = true;
}
