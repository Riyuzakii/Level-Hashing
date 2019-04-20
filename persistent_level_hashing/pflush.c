#include "pflush.h"
/* Note that we refered to the implementation code of pflush function in Quartz
*/
extern int con_method;

static inline unsigned long long asm_rdtsc(void)
{
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}

static inline unsigned long long asm_rdtscp(void)
{
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtscp" : "=a"(lo), "=d"(hi)::"rcx");
    return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}

static int global_cpu_speed_mhz = 0;
static int global_write_latency_ns = 0;

void init_pflush(int cpu_speed_mhz, int write_latency_ns)
{
    global_cpu_speed_mhz = cpu_speed_mhz;
    global_write_latency_ns = write_latency_ns;
}

uint64_t cycles_to_ns(int cpu_speed_mhz, uint64_t cycles)
{
    return (cycles*1000/cpu_speed_mhz);
}

uint64_t ns_to_cycles(int cpu_speed_mhz, uint64_t ns)
{
    return (ns*cpu_speed_mhz/1000);
}


static inline int emulate_latency_ns(int ns)
{
    if(ns < 0){
        return 0;
    }
        
    uint64_t cycles;
    uint64_t start;
    uint64_t stop;
    
    start = asm_rdtsc();
    cycles = ns_to_cycles(global_cpu_speed_mhz, ns);
    do { 
        stop = asm_rdtsc();
    } while (stop - start < cycles);
}

/*
Function: pflush() 
        Flush a cache line with the address addr;
*/
void pflush(uint64_t *addr)
{
    if (global_write_latency_ns == 0) {
        return;
    }

    /* Measure the latency of a clflush and add an additional delay to
       meet the write latency to NVM 
    */
    uint64_t start;
    uint64_t stop;
    // asm_clflush(addr);  
    
    // printf("CON: %d\n", con_method);
    if(con_method == 0){//SFENCE-FLUSH
        start = asm_rdtscp();
        asm volatile("sfence");
        asm_clflush(addr);
        stop = asm_rdtscp();
    }
    else if(con_method == 1){// MFENCE-FLUSH-MFENCE
        start = asm_rdtscp();
        asm volatile("mfence");
        asm_clflush(addr);
        asm volatile("mfence");
        stop = asm_rdtscp();
    }
    else if(con_method == 2 || con_method == 3){
        start = 0;
        stop = 0;
    }
    else if(con_method == 4){ //FLUSH WITH WT-CACHE
        start = asm_rdtscp();
        asm_clflush(addr);
        stop = asm_rdtscp();        
    }

    emulate_latency_ns(global_write_latency_ns - cycles_to_ns(global_cpu_speed_mhz, stop-start));
}