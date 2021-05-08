#pragma once

#include <gl/glew.h>
#include <future>

// Cube 1x1x1
const int cubeVerticesCount = 64;
const int cubeIndicesCount = 36;
unsigned int* GetCubeIndicesPtr();
float* GetCubeVerticesPtr();

void renderCube();
void renderQuad();

void Sleep(int seconds);