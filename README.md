
# README

## Intro

This is an implementation of a general circular queue.

## Usage

This is a basic usage example:

```c
// eg Struct or other type
struct Test {
    int a;
    char b;
    double c;
};

struct Test t1[32];
struct generic_fifo fifo1;
struct generic_fifo fifo2;

void foo(void)
{
    static Test *t2 = (struct Test *)malloc(32 * sizeof(struct Test));
    generic_fifo_init(&fifo1, t1, 0, 32);
    generic_fifo_init(&fifo2, t2, 1, 32);
    // Don't forget to free :)
}
```

