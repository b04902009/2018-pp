#define CHUNK 1024
#define uint32_t unsigned int
static inline uint32_t rotate_left(uint32_t x, uint32_t n){
    return  (x << n) | (x >> (32-n));
}
static inline uint32_t encrypt(uint32_t m, uint32_t key){
    return (rotate_left(m, key&31) + key)^key;
}
 
__kernel void VectorDot(uint32_t key1, uint32_t key2, __global int* C, int s, int e){
    int global_id = get_global_id(0);
    int group_id = get_group_id(0);
    int local_id = get_local_id(0);
    __local int buf[256];
 
    int start = s+global_id*CHUNK, end = start+CHUNK > e ? e : start+CHUNK;
    int sum = 0;
    for(int i = start; i < end; i++)
        sum += encrypt(i, key1) * encrypt(i, key2);
    buf[local_id] = sum;
    barrier(CLK_LOCAL_MEM_FENCE);
 
    for(int i = 128; i > 0; i >>= 1){
        if(local_id < i)
            buf[local_id] += buf[local_id+i];
        barrier(CLK_LOCAL_MEM_FENCE);
    }
    if(local_id == 0)
        C[group_id] = buf[0];
}