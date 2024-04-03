#include <cstdlib>
#include <ctime>
#include <ffiseg/random_generator.hpp>

using namespace ffiseg;

random_generator::random_generator() {
    seed(0);
}

random_generator::random_generator(unsigned seed) {
    random_generator::seed(seed);
}

void random_generator::seed(unsigned s) {
    if (s == 0) {
        s = (unsigned)clock();
    }

    for (unsigned i = 0; i < 17; i++) {
        s = s * 2891336453 + 1;
        buffer[i] = s;
    }

    p1 = 0;  p2 = 10;
}

unsigned* random_generator::get_buffer() {
    return buffer;
}

unsigned random_generator::rotl(unsigned n, unsigned r)
{
	  return (n << r) | (n >> (32 - r));
}

unsigned random_generator::rotr(unsigned n, unsigned r)
{
	  return (n >> r) | (n << (32 - r));
}

unsigned random_generator::random_bits() {
    unsigned result;

    // Rotate the buffer and store it back to itself
    result = buffer[p1] = rotl(buffer[p2], 13) + rotl(buffer[p1], 9);

    // Rotate pointers
    if (--p1 < 0) p1 = 16;
    if (--p2 < 0) p2 = 16;

    // Return result
    return result;
}

real random_generator::random_real()
{
    // Get the random number
    unsigned bits = random_bits();

    // Set up a reinterpret structure for manipulation
    union {
        real value;
        unsigned word;
    } convert;

    // Now assign the bits to the word. This works by fixing the ieee
    // sign and exponent bits (so that the size of the result is 1-2)
    // and using the bits to create the fraction part of the float.
    convert.word = (bits >> 9) | 0x3f800000;

    // And return the value
    return convert.value - 1.0f;
}

real random_generator::random_real(real min, real max) {
    return random_real() * (max-min) + min;
}

real random_generator::random_real(real scale) {
    return random_real() * scale;
}

unsigned random_generator::random_int(unsigned max) {
    return random_bits() % max;
}

real random_generator::random_binomial(real scale)
{
    return (random_real()-random_real())*scale;
}

vector random_generator::random_vector(real scale) {
    return vector(
        random_binomial(scale),
        random_binomial(scale),
        random_binomial(scale)
        );
}

vector random_generator::random_vector(const vector &scale) {
    return vector(
        random_binomial(scale.get_x()),
        random_binomial(scale.get_y()),
        random_binomial(scale.get_z())
        );
}

vector random_generator::random_vector(const vector &min, const vector &max) {
    return vector(
        random_real(min.get_x(), max.get_x()),
        random_real(min.get_y(), max.get_y()),
        random_real(min.get_z(), max.get_z())
        );
}
