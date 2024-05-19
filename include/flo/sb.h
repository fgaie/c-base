#ifndef BASE_SB_H
#define BASE_SB_H

#include <stdarg.h>

#include <flo.h>
#include <flo/mem.h>
#include <flo/sv.h>

typedef struct {
  char *data;
  u64 size;
  u64 cap;
} StringBuilder;

RES sb_pushfv(Arena *, StringBuilder *, const char *fmt, va_list);
RES sb_pushf(Arena *, StringBuilder *, const char *fmt, ...);

#define sb_push(a, sb, x)                                                      \
  _Generic((x),                                                                \
      StringView: sb_push_sv,                                                  \
      char: sb_push_char,                                                      \
      i64: sb_push_int,                                                        \
      u64: sb_push_unsigned,                                                   \
      f64: sb_push_float)(a, sb, x)

RES sb_push_sv(Arena *, StringBuilder *, StringView);
RES sb_push_char(Arena *, StringBuilder *, char);
RES sb_push_int(Arena *, StringBuilder *, i64);
RES sb_push_unsigned(Arena *, StringBuilder *, u64);
RES sb_push_float(Arena *, StringBuilder *, f64);

#endif // BASE_SB_H
