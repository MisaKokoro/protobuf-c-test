#include <stdio.h>
#include <stdlib.h>
#include "struct.pb-c.h"
#include "test.h"
#include "time.h"
#define MALLOC_SIZE (1024 * 1024 *1024)

void *heap_base;
void *heap_rear;
ProtobufCAllocator myallocator;
enum {
    SYSTEM_ALLOC,
    MY_ALLOC,
    SER,
};
void *myalloc(void *allocator_data,size_t size) {
    size = (size + 8 - 1) / 8 * 8;
    (void) allocator_data;
    if (heap_rear == NULL) {
        heap_rear = heap_base;
        heap_rear += size;
        return heap_base;
    }
    void *res = heap_rear;
    heap_rear += size;
    // printf("size = %d,res = %p\n",size,res);
    return res;
}

void myfree(void *allocator_data,void *ptr) {
    (void) allocator_data;
    return;
}

void Init_Alloc() {
    void *res = malloc(MALLOC_SIZE);
    heap_base = res;
    heap_rear = NULL;
    myallocator.alloc = &myalloc;
    myallocator.free = &myfree;
    myallocator.allocator_data = NULL;
}

ProtobufCAllocator *Alloctor[] = {NULL,&myallocator};
char *test_case[] = {"system_malloc","myalloc","ser"};
int cnt;
int id;
int main(int argc,char *argv[]) {
    id = atoi(argv[1]);
    cnt = atoi(argv[2]);
    // int id = 1;
    // cnt = 1;
    Init_Alloc();
    test_main(id);
    return 0;
}

void test_main(int id) {
    struct timespec beginTime;
    struct timespec endTime;

    clock_gettime(CLOCK_REALTIME, &beginTime);
    switch (id) {
        case SYSTEM_ALLOC:
        case MY_ALLOC:
            test_Alloc();
            break;
        case SER:
            test_ser();
            break;  
        default:
            break;
    }
    clock_gettime(CLOCK_REALTIME,&endTime);

    long long diff = (endTime.tv_sec-beginTime.tv_sec)*1000000000 + (endTime.tv_nsec - beginTime.tv_nsec);
    long long na = diff % 1000;
    long long micro = diff / 1000;
    printf("Alloc  test case: %s  cnt:%d \t\tcost:%lld.%03lldus\n",test_case[id],cnt,micro,na);

}

void test_Alloc() {
    ProtobufCAllocator *allocator = Alloctor[id];
    Unit test;
    unit__init(&test);
    test.a = 1;
    test.b = 2;
    test.c = "123456";
    char buf[1024];
    int size = unit__pack(&test,buf);
    for (int i = 0; i < cnt; i++) {
        Unit *p = unit__unpack(allocator,size,buf);
    //     printf("p->a = %d\n",p->a);
    //     printf("p->b = %ld\n",p->b);
    //     printf("p->c = %s\n",p->c);
    }
}

void test_ser() {
    for (int i = 0; i < cnt; i++) {
        Unit test;
        unit__init(&test);
        test.a = 1;
        test.b = 2;
        test.c = "123456";
        char buf[1024];
        int size = unit__pack(&test,buf);
    }
}