#ifndef BASE_H
#define BASE_H

#include <stdbool.h>
#include <stdint.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef unsigned int uint;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
#define clamp(x, a, b) min(b, min(a, x))

#define RES bool
#define OK true
#define ERR false
#define TRY(x)                                                                 \
  do {                                                                         \
    if (!(x)) {                                                                \
      return false;                                                            \
    }                                                                          \
  } while (0)

#endif // BASE_H
