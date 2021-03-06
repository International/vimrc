// RUN: %clangxx_tsan -O1 %s -o %t && not %run %t 2>&1 | FileCheck %s
#include <pthread.h>
#include <unistd.h>

int Global;

void *Thread1(void *x) {
  sleep(1);
  __atomic_fetch_add(&Global, 1, __ATOMIC_RELAXED);
  return NULL;
}

void *Thread2(void *x) {
  Global++;
  return NULL;
}

int main() {
  pthread_t t[2];
  pthread_create(&t[0], NULL, Thread1, NULL);
  pthread_create(&t[1], NULL, Thread2, NULL);
  pthread_join(t[0], NULL);
  pthread_join(t[1], NULL);
}

// CHECK: WARNING: ThreadSanitizer: data race
// CHECK:   Atomic write of size 4
// CHECK:     #0 __tsan_atomic32_fetch_add
// CHECK:     #1 Thread1
