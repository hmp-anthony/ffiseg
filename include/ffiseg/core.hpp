#include <ffiseg/precision.hpp>

namespace ffiseg {
    class vector {
    public:
        vector() : x(0), y(0), z(0) {}
        vector(const real x, const real y, const real z)
            : x(x), y(y), z(z) {}

        void operator*=(const real a) {
            x *= a;
            y *= a;
            z *= a;
        }

        vector operator*(const real a) const {
            return vector(a*x, a*y, a*z);
        }

        real get_x() { return x; }
        real get_y() { return y; }
        real get_z() { return z; }

        void set_x(real _x) { x = _x; }
        void set_y(real _y) { y = _y; }
        void set_z(real _z) { z = _z; }

        void invert() {
            x = -x;
            y = -y;
            z = -z;
        }

        real magnitude() const {
            return real_sqrt(x*x + y*y + z*z);
        }
    
        real square_magnitude() const {
            return x*x + y*y + z*z;
        }

    private:
        real x;
        real y;
        real z;
        real pad;
    };
};
