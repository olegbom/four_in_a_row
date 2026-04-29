#pragma once
#include <stdint.h>
#include "field.h"

typedef struct {
    uint64_t a, b;
} field3d_s;

void field3dTestMasksVisual();
void field3dPrint(field3d_s f);
