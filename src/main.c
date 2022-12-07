#include <stdio.h>
#include "struct.pb-c.h"

int main() {
    Unit test;
    unit__init(&test);
    test.a = 1;
    test.b = 2;
    test.c = "123456";
    char buf[1024];
    int size = unit__pack(&test,buf);

    Unit *p = unit__unpack(NULL,size,buf);
    printf("p->a = %d\n",p->a);
    printf("p->b = %ld\n",p->b);
    printf("p->c = %s\n",p->c);
    return 0;

}