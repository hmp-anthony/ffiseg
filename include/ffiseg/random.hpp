#ifndef RANDOM_H
#define RANDOM_H

#include<ffiseg/core.hpp>

namespace ffiseg {
    class random {
    public:
        random();
        random(unsigned seed);
        void seed(unsigned seed);
        unsigned rotl(unsigned n, unsigned r);
        unsigned rotr(unsigned n, unsigned r);
        unsigned random_bits();
        real random_real();
        real random_real(real scale);
        real random_real(real min, real max);
        unsigned random_int(unsigned max);
        real random_binomial(real scale);
        vector random_vector(real scale);
        vector random_vector(const vector& scale);
        vector random_vector(const vector& min, const vector& max);
    private:
        int p1, p2;
        unsigned buffer[17];
    };
}

#endif
