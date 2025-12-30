#ifndef XCAMERA_H
#define XCAMERA_H

#include <math.h>
#include "xMath.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct xCamera {
    Vec3 position;
    float yaw;      // Rotation in degrees
    float pitch;    // Rotation in degrees
    float fov;      // Field of view in degrees
    Vec3 front;     // Forward direction vector
    Vec3 right;     // Right direction vector
    Vec3 up;        // Up direction vector
} xCamera;

// Initialize camera with default position and orientation
/*  -> Example:
 *  xCamera camera;
 *  xCameraInit(&camera);
 *  camera.position = vec3(0.0f, 1.0f, 5.0f);
 */
void xCameraInit(xCamera *cam);

// Update camera vectors based on yaw/pitch (automatically clamps pitch)
/*  -> Example:
 *  camera.yaw += 10.0f;
 *  xCameraUpdate(&camera);
 */
void xCameraUpdate(xCamera *cam);

// Move camera in given direction by speed amount
/*  -> Example:
 *  if (xIsKeyDown(&input, KEY_W)) xCameraMove(&camera, camera.front, 0.1f);
 */
void xCameraMove(xCamera *cam, Vec3 direction, float speed);

// Rotate camera by delta angles in degrees
/*  -> Example:
 *  xCameraRotate(&camera, mouse_dx * sensitivity, -mouse_dy * sensitivity);
 */
void xCameraRotate(xCamera *cam, float dyaw, float dpitch);

// Generate ray from camera with pre-scaled viewport offsets
/*  -> Example:
 *  float* u_offsets = (float*)malloc(win.width * sizeof(float));
    float* v_offsets = (float*)malloc(win.height * sizeof(float));
    assert(u_offsets && v_offsets && "Failed to allocate viewport offset buffers");

    for (int x = 0; x < win.width; x++)  u_offsets[x] = ((float)x / (float)(win.width - 1) - 0.5f) * viewport_width;
    for (int y = 0; y < win.height; y++) v_offsets[y] = ((float)(win.height - 1 - y) / (float)(win.height - 1) - 0.5f) * viewport_height;

    while (1) {
        ...
        #pragma omp parallel for schedule(dynamic) default(none) shared(win, camera, u_offsets, v_offsets)
        for (int y = 0; y < win.height; y++) {
            for (int x = 0; x < win.width; x++) {
                const Ray ray = xCameraGetRay(&camera, u_offsets[x], v_offsets[y]);
                const Vec3 color = calculate_ray_color(ray, MAX_BOUNCES);
                win.buffer[y * win.width + x] = uint32(color);
        }
    }
}*/
Ray xCameraGetRay(const xCamera* cam, float u_scaled, float v_scaled);

#ifdef __cplusplus
}
#endif

#ifdef XCAMERA_IMPLEMENTATION

inline void xCameraInit(xCamera *cam)
{
    cam->position = vec3(0.0f, 0.0f, 0.0f);
    cam->yaw = 0.0f;
    cam->pitch = 0.0f;
    cam->fov = 60.0f;
    xCameraUpdate(cam);
}

inline void xCameraUpdate(xCamera *cam)
{
    // Clamp pitch to avoid gimbal lock
    if (cam->pitch > 89.0f) cam->pitch = 89.0f;
    if (cam->pitch < -89.0f) cam->pitch = -89.0f;

    const float yaw_rad = cam->yaw * M_PI / 180.0f;
    const float pitch_rad = cam->pitch * M_PI / 180.0f;

    cam->front = vec3(
        cosf(yaw_rad) * cosf(pitch_rad),
        sinf(pitch_rad),
        sinf(yaw_rad) * cosf(pitch_rad)
    );

    cam->front = norm(cam->front);
    cam->right = norm(cross(cam->front, vec3(0, 1, 0)));
    cam->up    = cross(cam->right, cam->front);
}

inline void xCameraMove(xCamera *cam, const Vec3 direction, const float speed)
{
    cam->position = add(cam->position, mul(direction, speed));
}

inline void xCameraRotate(xCamera *cam, const float dyaw, const float dpitch)
{
    cam->yaw += dyaw;
    cam->pitch += dpitch;
    xCameraUpdate(cam);
}

inline Ray xCameraGetRay(const xCamera* cam, const float u_scaled, const float v_scaled)
{
    Vec3 rd = add(cam->front, add(mul(cam->up, v_scaled), mul(cam->right, u_scaled)));
    rd = norm(rd);
    return (Ray){cam->position, rd};
}

#endif // XCAMERA_IMPLEMENTATION
#endif // XCAMERA_H