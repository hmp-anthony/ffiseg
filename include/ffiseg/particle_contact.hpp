#ifndef PARTICLE_CONTACT_H
#define PARTICLE_CONTACT_H

#include<ffiseg/particle.hpp>

namespace cyclone {

    //class particle_contact_resolver;

    class particle_contact {
        //friend class particle_contact_resolver;
    protected:
        void resolve(real duration);
        real calculate_separating_velocity() const;
    private:
        particle* parts[2];
        real restitution;
        vector contact_normal;
        real penetration;
        vector particle_movement[2];

        void resolve_velocity(real duration);
        void resolve_interpenetration(real duration);
    };

}

#endif
