#include "utils.h"
#define CACHE_LINESIZE 16
int ret[256];
int run(int n, int key) {
    f(n, key, ret, ret+CACHE_LINESIZE, ret+CACHE_LINESIZE*2, ret+CACHE_LINESIZE*3);
    return ret[0] + ret[CACHE_LINESIZE] + ret[CACHE_LINESIZE*2] + ret[CACHE_LINESIZE*3];
}