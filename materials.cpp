#include "Material.h"


struct material light1 {
    {0.1, 0.1, 0.1, 1},
    {0.1, 0.1, 0.1, 1},
    {0.1, 0.1, 0.1, 1},
    {0.0, 0.0, 0.0, 1.0},
    0
};

struct material white_glossy_paint = {
    {0.7, 0.7, 0.7, 1.0},
    {0.3, 0.3, 0.3, 1.0},
    {0.4, 0.4, 0.4, 1.0},
    {0.0, 0.0, 0.0, 1.0},
    1.5
};

struct material sun_material = {
    {1, 1, 0, 1.0},
    {1, 1, 0, 1.0},
    {0.5, 0.5, 0, 1.0},
    {0.4, 0.4, 0, 1.0},
    30
};

// default. Rather high ambient so stuff is visible in the darkess of space
struct material smooth_material = {
    {1, 1, 1, 1.0},
    {0.5, 0.5, 0.5, 1.0},
    {0.3, 0.3, 0.3, 1.0},
    {0.0, 0.0, 0.0, 1.0},
    3.0
};
