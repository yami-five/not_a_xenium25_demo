#include "IRenderer.h"
#include "renderer.h"

static volatile const IHardware *_hardware = NULL;
static volatile const IPainter *_painter = NULL;

static const uint8_t SCALE = 2;
static const uint8_t FOCAL_LENGTH = 90;
static const uint16_t WIDTH_DISPLAY = 320 / SCALE;
static const uint16_t HEIGHT_DISPLAY = 240 / SCALE;
static const uint16_t WIDTH_DOUBLED = 640 / SCALE;
static const uint16_t HEIGHT_DOUBLED = 480 / SCALE;
static const uint32_t ARRAY_SIZE = 153600;
static const uint16_t WIDTH_HALF = 160 / SCALE;
static const uint16_t HEIGHT_HALF = 120 / SCALE;
static const uint32_t FIRE_FLOOR_ADR = 76480;
static const uint32_t FIXED_FOCAL_LENGTH = 90 * SCALE_FACTOR;
static const uint32_t TRIANGLE_CENTER_DIVIDER = 3 * SCALE_FACTOR;
static const uint16_t ZBUFFERSIZE = 19200; // 160*120
static uint16_t zBuffer[19200];

#define SHADING_ENABLED 1

void clear_zbuffuer();

void init_renderer(volatile const IHardware *hardware, volatile const IPainter *painter)
{
    _hardware = hardware;
    _painter = painter;
    clear_zbuffuer();
    // init_sin_cos();
}

void triangle_center(Triangle3D *triangle, int32_t *center)
{
    center[0] = fixed_div((triangle->a.x + triangle->b.x + triangle->c.x), TRIANGLE_CENTER_DIVIDER);
    center[1] = fixed_div((triangle->a.y + triangle->b.y + triangle->c.y), TRIANGLE_CENTER_DIVIDER);
    center[2] = fixed_div((triangle->a.z + triangle->b.z + triangle->c.z), TRIANGLE_CENTER_DIVIDER);
}

void rotate(int32_t *x, int32_t *y, int32_t *z, TransformVector *vector)
{
    if (vector->x != 0)
    {
        int32_t qt_rad = fixed_mul(vector->x, PI2_FIXED);
        int32_t c = fast_cos(qt_rad);
        int32_t s = fast_sin(qt_rad);
        int32_t temp_x = fixed_mul(c, *y) - fixed_mul(s, *z);
        int32_t temp_y = fixed_mul(s, *y) + fixed_mul(c, *z);
        *y = temp_x;
        *z = temp_y;
    }
    if (vector->y != 0)
    {
        int32_t qt_rad = fixed_mul(vector->y, PI2_FIXED);
        int32_t c = fast_cos(qt_rad);
        int32_t s = fast_sin(qt_rad);
        int32_t temp_x = fixed_mul(c, *x) - fixed_mul(s, *z);
        int32_t temp_y = fixed_mul(s, *x) + fixed_mul(c, *z);
        *x = temp_x;
        *z = temp_y;
    }
    if (vector->z != 0)
    {
        int32_t qt_rad = fixed_mul(vector->z, PI2_FIXED);
        int32_t c = fast_cos(qt_rad);
        int32_t s = fast_sin(qt_rad);
        int32_t temp_x = fixed_mul(c, *x) - fixed_mul(s, *y);
        int32_t temp_y = fixed_mul(s, *x) + fixed_mul(c, *y);
        *x = temp_x;
        *y = temp_y;
    }
}

void translate(int32_t *x, int32_t *y, int32_t *z, TransformVector *vector)
{
    *x += vector->x;
    *y += vector->y;
    *z += vector->z;
}

void scale(int32_t *x, int32_t *y, int32_t *z, TransformVector *vector)
{
    *x = fixed_mul(*x, vector->x);
    *y = fixed_mul(*y, vector->y);
    *z = fixed_mul(*z, vector->z);
}

void transform(int32_t *x, int32_t *y, int32_t *z, TransformInfo *transformInfo)
{
    if (transformInfo->transformType == 0)
        rotate(x, y, z, transformInfo->transformVector);
    if (transformInfo->transformType == 1)
        translate(x, y, z, transformInfo->transformVector);
    if (transformInfo->transformType == 2)
        scale(x, y, z, transformInfo->transformVector);
}

void inf(float *x, float *y, float qt)
{
    float qt_rad = qt * PI2;
    *x += 2.0f * (fast_cos(qt_rad));
    *y += 2.0f * fast_cos(qt_rad) * fast_sin(qt_rad);
}

int check_if_triangle_visible(Triangle2D *triangle)
{
    int e1x = triangle->b.x - triangle->a.x;
    int e1y = triangle->b.y - triangle->a.y;
    int e2x = triangle->c.x - triangle->a.x;
    int e2y = triangle->c.y - triangle->a.y;

    return (e1x * e2y - e1y * e2x) >= 0;
}

void shading(uint16_t *color, int32_t lightDistances[], PointLight *light, int Ba, int Bb, int Bc)
{

    uint8_t rMesh = (*color >> 11) & 0x1f;
    uint8_t gMesh = (*color >> 5) & 0x3f;
    uint8_t bMesh = *color & 0x1f;

    uint8_t rLight = (light->color >> 11) & 0x1f;
    uint8_t gLight = (light->color >> 5) & 0x3f;
    uint8_t bLight = light->color & 0x1f;

    uint8_t r = (rMesh * rLight) / 31;
    uint8_t g = (gMesh * gLight) / 63;
    uint8_t b = (bMesh * bLight) / 31;

    uint32_t fixedR = r * SCALE_FACTOR;
    uint32_t fixedG = g * SCALE_FACTOR;
    uint32_t fixedB = b * SCALE_FACTOR;

    int32_t lightDistance = (fixed_mul(Ba, lightDistances[0]) + fixed_mul(Bb, lightDistances[1]) + fixed_mul(Bc, lightDistances[2]));
    if (lightDistance < 0)
        lightDistance = 0;

    fixedR = fixed_mul(fixedR, lightDistance);
    fixedG = fixed_mul(fixedG, lightDistance);
    fixedB = fixed_mul(fixedB, lightDistance);

    r = (uint8_t)(fixed_mul(fixedR, light->intensity) / SCALE_FACTOR);
    g = (uint8_t)(fixed_mul(fixedG, light->intensity) / SCALE_FACTOR);
    b = (uint8_t)(fixed_mul(fixedB, light->intensity) / SCALE_FACTOR);

    *color = (r << 11) | (g << 5) | b;
}

uint16_t texturing(Triangle2D *triangle, Material *mat, int Ba, int Bb, int Bc)
{
    int uv_x = (fixed_mul(Ba, triangle->uvA.x) + fixed_mul(Bb, triangle->uvB.x) + fixed_mul(Bc, triangle->uvC.x)) * mat->textureSize;
    int uv_y = (fixed_mul(Ba, triangle->uvA.y) + fixed_mul(Bb, triangle->uvB.y) + fixed_mul(Bc, triangle->uvC.y)) * mat->textureSize;
    uv_x = uv_x / SCALE_FACTOR;
    uv_y = uv_y / SCALE_FACTOR;
    if (uv_x < 0)
        uv_x = 0;
    if (uv_x > mat->textureSize)
        uv_x = mat->textureSize - 1;
    if (uv_y < 0)
        uv_y = 0;
    if (uv_y > mat->textureSize)
        uv_y = mat->textureSize - 1;
    return mat->texture[uv_y * mat->textureSize + uv_x];
}

int calc_pixel_depth(int Ba, int Bb, int Bc, int z1, int z2, int z3)
{
    int z = fixed_mul(Ba, fixed_div(SCALE_FACTOR, z1)) + fixed_mul(Bb, fixed_div(SCALE_FACTOR, z2)) + fixed_mul(Bc, fixed_div(SCALE_FACTOR, z3));
    return fixed_div(SCALE_FACTOR, z) / SCALE_FACTOR;
}

void calc_bar_coords(Triangle2D *triangle, int *Ba, int *Bb, int *Bc, int32_t divider, int x, int y)
{
    int tempBa, tempBb, tempBc;
    tempBa = ((triangle->b.y - triangle->c.y) * (x - triangle->c.x) + (triangle->c.x - triangle->b.x) * (y - triangle->c.y)) * SCALE_FACTOR;
    tempBb = ((triangle->c.y - triangle->a.y) * (x - triangle->c.x) + (triangle->a.x - triangle->c.x) * (y - triangle->c.y)) * SCALE_FACTOR;
    tempBa = fixed_div(tempBa, divider);
    tempBb = fixed_div(tempBb, divider);
    tempBc = SCALE_FACTOR - tempBa - tempBb;
    *Ba = tempBa;
    *Bb = tempBb;
    *Bc = tempBc;
}

void clear_zbuffuer()
{
    memset(zBuffer, 36865, sizeof(zBuffer));
}

void set_zbuffer(uint16_t addr, int z)
{
    zBuffer[addr] = z;
}

uint8_t check_zbuffer(int x, int y, int z)
{
    uint16_t addr = x * HEIGHT_DISPLAY + y;
    if (z < zBuffer[addr])
    {
        set_zbuffer(addr, z);
        return 1;
    }
    else
        return 0;
}

void rasterize(int y, int x0, int x1, Triangle2D *triangle, Material *mat, int32_t lightDistances[], int32_t divider, PointLight *light)
{
    if (y < 0 || y >= HEIGHT_DISPLAY)
        return;
    int n = (y % 2) / 2;
    x0 += n;
    x1 += n;
    int q;
    if (x1 < x0)
    {
        q = x0;
        x0 = x1;
        x1 = q;
    }
    if (x1 < 0 || x0 >= WIDTH_DISPLAY)
        return;
    if (x0 < 0)
        x0 = 0;
    if (x1 > WIDTH_DISPLAY)
        x1 = WIDTH_DISPLAY;
    for (int x = x0; x < x1; x++)
    {
        uint16_t color = 0;
        int Ba, Bb, Bc;
        calc_bar_coords(triangle, &Ba, &Bb, &Bc, divider, x, y);
        int z = calc_pixel_depth(Ba, Bb, Bc, triangle->a.z, triangle->b.z, triangle->c.z);
        if (check_zbuffer(x, y, z))
        {
            if (mat->textureSize == 0)
                color = mat->diffuse;
            else
                color = texturing(triangle, mat, Ba, Bb, Bc);
            if (SHADING_ENABLED)
                shading(&color, lightDistances, light, Ba, Bb, Bc);
            _painter->draw_pixel(x * 2, y * 2, color);
            _painter->draw_pixel(x * 2 + 1, y * 2, color);
            _painter->draw_pixel(x * 2, y * 2 + 1, color);
            _painter->draw_pixel(x * 2 + 1, y * 2 + 1, color);
            // draw_pixel(x, y, color);
        }
    }
}

void tri(Triangle2D *triangle, Material *mat, int32_t lightDistances[], PointLight *light)
{
    int x, y, z, uv;
    if (triangle->a.y > triangle->b.y)
    {
        z = triangle->a.z;
        triangle->a.z = triangle->b.z;
        triangle->b.z = z;
        y = triangle->a.y;
        triangle->a.y = triangle->b.y;
        triangle->b.y = y;
        x = triangle->a.x;
        triangle->a.x = triangle->b.x;
        triangle->b.x = x;

        uv = triangle->uvA.x;
        triangle->uvA.x = triangle->uvB.x;
        triangle->uvB.x = uv;
        uv = triangle->uvA.y;
        triangle->uvA.y = triangle->uvB.y;
        triangle->uvB.y = uv;
    }
    if (triangle->a.y > triangle->c.y)
    {
        z = triangle->a.z;
        triangle->a.z = triangle->c.z;
        triangle->c.z = z;
        y = triangle->a.y;
        triangle->a.y = triangle->c.y;
        triangle->c.y = y;
        x = triangle->a.x;
        triangle->a.x = triangle->c.x;
        triangle->c.x = x;

        uv = triangle->uvA.x;
        triangle->uvA.x = triangle->uvC.x;
        triangle->uvC.x = uv;
        uv = triangle->uvA.y;
        triangle->uvA.y = triangle->uvC.y;
        triangle->uvC.y = uv;
    }
    if (triangle->b.y > triangle->c.y)
    {
        z = triangle->b.z;
        triangle->b.z = triangle->c.z;
        triangle->c.z = z;
        y = triangle->b.y;
        triangle->b.y = triangle->c.y;
        triangle->c.y = y;
        x = triangle->b.x;
        triangle->b.x = triangle->c.x;
        triangle->c.x = x;

        uv = triangle->uvB.x;
        triangle->uvB.x = triangle->uvC.x;
        triangle->uvC.x = uv;
        uv = triangle->uvB.y;
        triangle->uvB.y = triangle->uvC.y;
        triangle->uvC.y = uv;
    }
    if (triangle->c.y < 0 || triangle->a.y > HEIGHT_DISPLAY)
        return;
    y = triangle->a.y;
    int xx = x = triangle->a.x;

    int dx01 = triangle->b.x - triangle->a.x;
    int dy01 = triangle->b.y - triangle->a.y;

    int dx02 = triangle->c.x - triangle->a.x;
    int dy02 = triangle->c.y - triangle->a.y;

    int dx12 = triangle->c.x - triangle->b.x;
    int dy12 = triangle->c.y - triangle->b.y;

    int q2 = 0;
    int xxd = 1;

    if (triangle->c.x < triangle->a.x)
        xxd = -1;

    int32_t divider = 0;

    if (mat->textureSize > 0)
    {
        divider = (triangle->b.y - triangle->c.y) * (triangle->a.x - triangle->c.x) + (triangle->c.x - triangle->b.x) * (triangle->a.y - triangle->c.y);
        divider *= SCALE_FACTOR;
        // if (divider < 0)
        //     divider = -divider;
    }

    if (triangle->a.y < triangle->b.y)
    {
        int q = 0;
        int xd = 1;
        if (triangle->b.x < triangle->a.x)
            xd = -1;
        while (y <= triangle->b.y)
        {
            rasterize(y, x, xx, triangle, mat, lightDistances, divider, light);
            y += 1;
            q += dx01;
            q2 += dx02;
            while (xd * q >= dy01)
            {
                q -= xd * dy01;
                x += xd;
            }
            while (xxd * q2 >= dy02)
            {
                q2 -= xxd * dy02;
                xx += xxd;
            }
        }
    }

    if (triangle->b.y < triangle->c.y)
    {
        int q = 0;
        int x = triangle->b.x;
        int xd = 1;
        if (triangle->c.x < triangle->b.x)
            xd = -1;

        while (y <= triangle->c.y && y < HEIGHT_DISPLAY)
        {
            rasterize(y, x, xx, triangle, mat, lightDistances, divider, light);
            y += 1;
            q += dx12;
            q2 += dx02;
            while (xd * q > dy12)
            {
                q -= xd * dy12;
                x += xd;
            }
            while (xxd * q2 > dy02)
            {
                q2 -= xxd * dy02;
                xx += xxd;
            }
        }
    }
}

void draw_model(Mesh *mesh, PointLight *pLight, Camera *camera)
{
    uint16_t verticesCounter = mesh->verticesCounter;
    int verticesModified[verticesCounter * 3];
    int verticesOnScreen[verticesCounter * 3];
    int normalsModified[verticesCounter * 3];
    for (uint16_t i = 0; i < verticesCounter * 3; i += 3)
    {
        // calculates vertex coords in 3D space
        int32_t x = mesh->vertices[i];
        int32_t y = mesh->vertices[i + 1];
        int32_t z = mesh->vertices[i + 2];
        for (int j = 0; j < mesh->transformationsNum; j++)
        {
            transform(&x, &y, &z, &mesh->transformations[j]);
        }
        // calculates vertex coords on the screen
        verticesModified[i] = x;
        verticesModified[i + 1] = y;
        verticesModified[i + 2] = z;
        int32_t w = SCALE_FACTOR;
        z -= (5 * SCALE_FACTOR * 5);
        fixed_mul_matrix_vector(&x, &y, &z, &w, camera->vMatrix);
        fixed_mul_matrix_vector(&x, &y, &z, &w, camera->pMatrix);
        // z += (5 * SCALE_FACTOR);
        // x = (x * FIXED_FOCAL_LENGTH / z) + (SCALE_FACTOR * WIDTH_HALF);
        // y = (y * FIXED_FOCAL_LENGTH / z) + (SCALE_FACTOR * HEIGHT_HALF);
        // verticesOnScreen[vsc] = x / SCALE_FACTOR;
        // verticesOnScreen[vsc + 1] = y / SCALE_FACTOR;
        verticesOnScreen[i] = fixed_div(x, w) + WIDTH_HALF;
        verticesOnScreen[i + 1] = fixed_div(y, w) + HEIGHT_HALF;
        verticesOnScreen[i + 2] = z;
        int32_t xn = mesh->normals[i];
        int32_t yn = mesh->normals[i + 1];
        int32_t zn = mesh->normals[i + 2];
        for (int j = 0; j < mesh->transformationsNum; j++)
        {
            if (&mesh->transformations[j].transformType == 0)
                transform(&xn, &yn, &zn, &mesh->transformations[j]);
        }
        normalsModified[i] = xn;
        normalsModified[i + 1] = yn;
        normalsModified[i + 2] = zn;
    }
    // flat shading
    Vector3 normalVectorA;
    Vector3 normalVectorB;
    Vector3 normalVectorC;
    Vector3 lightDirectionA;
    Vector3 lightDirectionB;
    Vector3 lightDirectionC;
    // Vector3 lmn;
    // Vector3 *normalVector = (Vector3 *)malloc(sizeof(Vector3));
    // Vector3 *lightDirection = (Vector3 *)malloc(sizeof(Vector3));
    // Vector3 *lmn = (Vector3 *)malloc(sizeof(Vector3));
    for (uint16_t i = 0; i < mesh->facesCounter * 3; i += 3)
    {
        uint16_t a = mesh->faces[i];
        uint16_t b = mesh->faces[i + 1];
        uint16_t c = mesh->faces[i + 2];
        uint16_t uvA = mesh->uv[i];
        uint16_t uvB = mesh->uv[i + 1];
        uint16_t uvC = mesh->uv[i + 2];
        Triangle2D triangle =
            {
                {verticesOnScreen[a * 3],
                 verticesOnScreen[a * 3 + 1],
                 verticesOnScreen[a * 3 + 2]},
                {verticesOnScreen[b * 3],
                 verticesOnScreen[b * 3 + 1],
                 verticesOnScreen[b * 3 + 2]},
                {verticesOnScreen[c * 3],
                 verticesOnScreen[c * 3 + 1],
                 verticesOnScreen[c * 3 + 2]},
                {mesh->textureCoords[uvA * 2],
                 mesh->textureCoords[uvA * 2 + 1]},
                {mesh->textureCoords[uvB * 2],
                 mesh->textureCoords[uvB * 2 + 1]},
                {mesh->textureCoords[uvC * 2],
                 mesh->textureCoords[uvC * 2 + 1]}};
        if (!check_if_triangle_visible(&triangle))
            continue;
        // normal vector
        int32_t lightDistances[3] = {0, 0, 0};
#ifdef SHADING_ENABLED
        normalVectorA.x = normalsModified[a * 3];
        normalVectorA.y = normalsModified[a * 3 + 1];
        normalVectorA.z = normalsModified[a * 3 + 2];
        normalVectorB.x = normalsModified[b * 3];
        normalVectorB.y = normalsModified[b * 3 + 1];
        normalVectorB.z = normalsModified[b * 3 + 2];
        normalVectorC.x = normalsModified[c * 3];
        normalVectorC.y = normalsModified[c * 3 + 1];
        normalVectorC.z = normalsModified[c * 3 + 2];

        int32_t normalVectorALength = len_vector(&normalVectorA);
        int32_t normalVectorBLength = len_vector(&normalVectorB);
        int32_t normalVectorCLength = len_vector(&normalVectorC);
        // light direction
        Triangle3D triangle3D = {
            {verticesModified[a * 3],
             verticesModified[a * 3 + 1],
             verticesModified[a * 3 + 2]},
            {verticesModified[b * 3],
             verticesModified[b * 3 + 1],
             verticesModified[b * 3 + 2]},
            {verticesModified[c * 3],
             verticesModified[c * 3 + 1],
             verticesModified[c * 3 + 2]}};

        lightDirectionA.x = pLight->position.x - normalVectorA.x;
        lightDirectionA.y = pLight->position.y - normalVectorA.y;
        lightDirectionA.z = pLight->position.z - normalVectorA.z;
        lightDirectionB.x = pLight->position.x - normalVectorB.x;
        lightDirectionB.y = pLight->position.y - normalVectorB.y;
        lightDirectionB.z = pLight->position.z - normalVectorB.z;
        lightDirectionC.x = pLight->position.x - normalVectorC.x;
        lightDirectionC.y = pLight->position.y - normalVectorC.y;
        lightDirectionC.z = pLight->position.z - normalVectorC.z;

        int32_t lightLengthA = len_vector(&lightDirectionA);
        int32_t lightLengthB = len_vector(&lightDirectionB);
        int32_t lightLengthC = len_vector(&lightDirectionC);

        // light distance
        Vector3 *lmnA = sub_vectors(&lightDirectionA, &normalVectorA);
        Vector3 *lmnB = sub_vectors(&lightDirectionB, &normalVectorB);
        Vector3 *lmnC = sub_vectors(&lightDirectionC, &normalVectorC);

        int64_t lightDirectionMinusNormalVectorA = len_vector(lmnA);
        int64_t lightDirectionMinusNormalVectorB = len_vector(lmnB);
        int64_t lightDirectionMinusNormalVectorC = len_vector(lmnC);

        free(lmnA);
        free(lmnB);
        free(lmnC);

        int32_t x = fixed_pow(normalVectorALength) + fixed_pow(lightLengthA) - fixed_pow(lightDirectionMinusNormalVectorA);
        int32_t y = fixed_mul(lightLengthA, normalVectorALength) * 2;
        lightDistances[0] = fixed_div(x, y);
        if (lightDistances[0] < 0)
            lightDistances[0] = 0;
        if (lightDistances[0] > SCALE_FACTOR)
            lightDistances[0] = SCALE_FACTOR;

        x = fixed_pow(normalVectorBLength) + fixed_pow(lightLengthB) - fixed_pow(lightDirectionMinusNormalVectorB);
        y = fixed_mul(lightLengthB, normalVectorBLength) * 2;
        lightDistances[1] = fixed_div(x, y);
        if (lightDistances[1] < 0)
            lightDistances[1] = 0;
        if (lightDistances[1] > SCALE_FACTOR)
            lightDistances[1] = SCALE_FACTOR;

        x = fixed_pow(normalVectorCLength) + fixed_pow(lightLengthC) - fixed_pow(lightDirectionMinusNormalVectorC);
        y = fixed_mul(lightLengthC, normalVectorCLength) * 2;
        lightDistances[2] = fixed_div(x, y);
        if (lightDistances[2] < 0)
            lightDistances[2] = 0;
        if (lightDistances[2] > SCALE_FACTOR)
            lightDistances[2] = SCALE_FACTOR;

#endif
        tri(&triangle, mesh->mat, lightDistances, pLight);
    }
    // free(normalVector);
    // free(lightDirection);
    // free(lmn);
}

static IRenderer renderer = {
    .init_renderer = init_renderer,
    .draw_model = draw_model,
    .clear_zbuffer = clear_zbuffuer};

const IRenderer *get_renderer(void)
{
    return &renderer;
}