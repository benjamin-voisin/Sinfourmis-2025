#pragma once

#include "stdlib.h"

void* Malloc(size_t size);

void* Calloc(size_t nmemb, size_t size);

void* Realloc(void* ptr, size_t size);

void* Reallocarray(void* ptr, size_t nmemb, size_t size);