#include <unistd.h>
#include <stdint.h>
#include <math.h>
#include <x11.h>
#include <xMath.h>

#define XTITLE "X11 Minimal FPS"
#define XFPS 60

static inline uint32_t ray_color(Vec3 o, Vec3 d)
{
    if (fabsf(d.y) < 0.001f) return 0x000000;

    float t = -o.y / d.y;
    if (t < 0.0f) return 0x000000;

    Vec3 h = add(o, mul(d, t));
    int c = ((int)floorf(h.x) + (int)floorf(h.z)) & 1;

    float f = fmaxf(0.0f, 1.0f - t / 40.0f);
    uint32_t b = c ? 0x404040 : 0x808080;

    return ((int)(((b >> 16) & 0xFF) * f) << 16) |
           ((int)(((b >>  8) & 0xFF) * f) <<  8) |
           ((int)(((b      ) & 0xFF) * f));
}

int main(void)
{
    xWindow win;
    xInit(&win);
    win.title = XTITLE;
    xCreateWindow(&win);

    Vec3 pos  = vec3(0, 1.6f, 5);
    float yaw = -90.0f, pitch = 0.0f;

    const float mv = 0.05f;
    const float rt = 1.5f;

    const float vh = 2.0f;
    const float vw = (float)win.width / win.height * vh;

    while (1)
    {
        if (xPollEvents(win.display)) break;
        if (xIsKeyPressed(Escape))    break;

        if (xIsKeyDown(Left))  yaw   -= rt;
        if (xIsKeyDown(Right)) yaw   += rt;
        if (xIsKeyDown(Up))    pitch += rt;
        if (xIsKeyDown(Down))  pitch -= rt;

        if (pitch >  89.0f) pitch =  89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        float py = pitch * M_PI / 180.0f;
        float yw = yaw   * M_PI / 180.0f;

        Vec3 front = norm(vec3(
            cosf(yw) * cosf(py),
            sinf(py),
            sinf(yw) * cosf(py)
        ));

        Vec3 right = norm(cross(front, vec3(0, 1, 0)));
        Vec3 up    = cross(right, front);

        if (xIsKeyDown(W)) pos = add(pos, mul(front,  mv));
        if (xIsKeyDown(S)) pos = add(pos, mul(front, -mv));
        if (xIsKeyDown(A)) pos = add(pos, mul(right, -mv));
        if (xIsKeyDown(D)) pos = add(pos, mul(right,  mv));

        for (int y = 0; y < win.height; y++) 
        {
            float v = ((float)(win.height - 1 - y) / (win.height - 1) - 0.5f) * vh;

            for (int x = 0; x < win.width; x++) 
            {
                float u = ((float)x / (win.width - 1) - 0.5f) * vw;
                Vec3 d = norm(add(front, add(mul(right, u), mul(up, v))));
                win.buffer[y * win.width + x] = ray_color(pos, d);
            }
        }

        xUpdateFramebuffer(&win);
        xUpdateInput();
        usleep(1000000 / XFPS);
    }

    xDestroyWindow(&win);
    return 0;
}

