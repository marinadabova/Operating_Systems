#include <stdio.h>

struct baba {
        int foo;
};

typedef struct {
        int bar;
} dyado_t;

int main(void) {
        struct baba yaga;
        yaga.foo = 42;

        dyado_t pesho;
        pesho.bar = 26;

        dprintf(1, "foo: %d, bar: %d\n", yaga.foo, pesho.bar);
}
