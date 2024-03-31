#include <ffiseg/precision.hpp>

namespace ffiseg {
    class vector {
    public:
        vector() : x(0), y(0), z(0) {}
        vector(const real x, const real y, const real z)
            : x(x), y(y), z(z) {}

        void invert() {
            x = -x;
            y = -y;
            z = -z;
        }
    private:
        real x;
        real y;
        real z;
        real pad;
    };
};
