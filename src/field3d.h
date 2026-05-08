#pragma once
#include <stdint.h>
#include "field.h"

typedef struct {
    uint64_t a, b;
} field3d_s;


void field3dTestMasksVisual();
bool field3dMoveAvailable( field3d_s f, uint8_t tube );
bool tryField3dMove( field3d_s *f, uint8_t player1_tube, uint8_t player2_tube );
void field3dPrint( field3d_s f, uint8_t tube );
