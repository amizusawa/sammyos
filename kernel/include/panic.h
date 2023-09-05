#ifndef PANIC_H
#define PANIC_H

#define PANIC(x) kpanic(x)

void kpanic(char* reason);

#endif
