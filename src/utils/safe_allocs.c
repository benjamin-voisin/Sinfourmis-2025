#include "safe_allocs.h"

#include "log.h"

void* Malloc(size_t size) {
    void* result = malloc(size);
    if (!result)
        Fatal(CAT_ALLOC, "Un malloc(%lu) a échoué.", size);
    return result;
}

void* Calloc(size_t nmemb, size_t size) {
    void* result = calloc(nmemb, size);
    if (!result)
        Fatal(CAT_ALLOC, "Un calloc(%lu, %lu) a échoué.", size, nmemb);
    return result;
}

void* Realloc(void* ptr, size_t size) {
    void* result = realloc(ptr, size);
    if (!result)
        Fatal(CAT_ALLOC, "Un realloc(%x, %lu) a échoué.", ptr, size);
    return result;
}

void* Reallocarray(void* ptr, size_t nmemb, size_t size) {
    void* result = reallocarray(ptr, nmemb, size);
    if (!result)
        Fatal(CAT_ALLOC, "Un reallocarray(%x, %lu, %lu) a échoué.", ptr, nmemb, size);
    return result;
}