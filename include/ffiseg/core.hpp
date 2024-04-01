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

        void operator+=(const vector& v) {
            x += v.get_x();
            y += v.get_y();
            z += v.get_z();
        }

        vector operator+(const vector& v) const {
            return vector(x + v.get_x(), y + v.get_y(), z + v.get_z()); 
        }

        void operator-=(const vector& v) {
            x -= v.get_x();
            y -= v.get_y();
            z -= v.get_z();
        }

        vector operator-(const vector& v) const {
            return vector(x - v.get_x(), y - v.get_y(), z - v.get_z());
        }

        real operator*(const vector& v) const {
            return x * v.get_x() + y * v.get_y() + z * v.get_z();
        }

        void operator %=(const vector& v) {
            *this = vector_product(v);
        }

        vector operator%(const vector& v) const {
            return vector(y * v.get_z() - z * v.get_y(),
                          z * v.get_x() - x * v.get_z(),
                          x * v.get_y() - y * v.get_x());
        }

        real get_x() const { return x; }
        real get_y() const { return y; }
        real get_z() const { return z; }

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

        void normalize() {
            real m = magnitude();
            if(m > 0) {
                (*this) *= 1.0/m; 
            }
        }

        void add_scaled_vector(const vector& v, real scale) {
            x += v.get_x() * scale;
            y += v.get_y() * scale;
            z += v.get_z() * scale;
        }
    
        vector component_product(const vector& v) const {
            return vector(x * v.get_x(), y * v.get_y(), z * v.get_z());
        }

        void component_product_update(const vector& v) {
            x *= v.get_x();
            y *= v.get_y();
            z *= v.get_z();
        }

        real scalar_product(const vector& v) const {
            return x * v.get_x() + y * v.get_y() + z * v.get_z();
        }

        vector vector_product(const vector& v) const {
            return vector(y * v.get_z() - z * v.get_y(),
                          z * v.get_x() - x * v.get_z(),
                          x * v.get_y() - y * v.get_x());
        }

        void clear() {
            set_x(0);
            set_y(0);
            set_z(0);
        }
    private:
        real x;
        real y;
        real z;
        real pad;
    };
};
