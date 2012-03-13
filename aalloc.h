#ifndef AALLOC_H
#define AALLOC_H
#include <cstdlib>
#include <cassert>

// SSE requires 16-bytes alignment so we have to be carefull when
// allocating any structure containig vector on the heap.

#ifdef _WIN32
inline void* aalloc(size_t size, size_t alignment)
{
    return _aligned_malloc(size, alignment);
}

inline void afree(void *ptr)
{
    _aligned_free(ptr);
}
#elif ANDROID
inline void* aalloc(size_t size, size_t alignment)
{
    return memalign(alignment, size);
}

inline void afree(void *ptr)
{
    free(ptr);
}
#else
inline void* aalloc(size_t size, size_t alignment)
{
    void *ptr;
    int ret = posix_memalign(&ptr, alignment, size);
    assert(ret == 0);
    return ptr;
}

inline void afree(void *ptr)
{
    free(ptr);
}
#endif

#endif
