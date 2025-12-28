#ifndef XMATH_H
#define XMATH_H

#include <math.h>

typedef struct {
    float x, y, z;
} Vec3;

typedef struct {
    Vec3 origin;
    Vec3 direction;
} Ray;

static inline Vec3 vec3(const float x, const float y, const float z) {
    return (Vec3){x, y, z};
}

static inline Vec3 add(const Vec3 a, const Vec3 b) {
    return (Vec3){a.x + b.x, a.y + b.y, a.z + b.z};
}

static inline Vec3 sub(const Vec3 a, const Vec3 b) {
    return (Vec3){a.x - b.x, a.y - b.y, a.z - b.z};
}

static inline Vec3 mul(const Vec3 v, const float t) {
    return (Vec3){v.x * t, v.y * t, v.z * t};
}

static inline Vec3 vdiv(const Vec3 v, const float t) {
    return (Vec3){v.x / t, v.y / t, v.z / t};
}

static inline float dot(const Vec3 a, const Vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline float len(const Vec3 v) {
    return sqrtf(dot(v, v));
}

static inline Vec3 norm(const Vec3 v) {
    return vdiv(v, len(v));
}

static inline Vec3 reflect(const Vec3 v, const Vec3 n) {
    return sub(v, mul(n, 2.0f * dot(v, n)));
}

static inline Vec3 cross(const Vec3 a, const Vec3 b) {
    return (Vec3){
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

#endif // XMATH_H
