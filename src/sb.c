#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <flo.h>
#include <flo/mem.h>
#include <flo/sv.h>

#include <flo/sb.h>

#define SB_MIN_CAP 8

static RES sb_grow(Arena *a, StringBuilder *sb, u64 want) {
  if (sb->cap > want) {
    return OK;
  }

  u64 new_cap = sb->cap > 0 ? sb->cap : SB_MIN_CAP;
  while (new_cap < want) {
    new_cap *= 2;
  }

  char *new_data = arena_realloc(a, sb->data, new_cap);
  if (new_data == NULL) {
    return ERR;
  }

  sb->cap = new_cap;
  sb->data = new_data;
  return OK;
}

RES sb_push_sv(Arena *a, StringBuilder *sb, StringView s) {
  TRY(sb_grow(a, sb, sb->size + s.size));

  memcpy(&sb->data[sb->size], s.data, s.size);
  sb->size += s.size;

  return OK;
}

RES sb_push_char(Arena *a, StringBuilder *sb, char c) {
  TRY(sb_grow(a, sb, sb->size + 1));

  sb->data[sb->size] = c;
  sb->size += 1;

  return OK;
}

RES sb_push_int(Arena *a, StringBuilder *sb, i64 n) {
  u64 size = n < 0 ? 1 : 0;
  for (i64 m = n; m != 0; m /= 10) {
    size += 1;
  }

  TRY(sb_grow(a, sb, sb->size + size));

  if (n < 0) {
    sb->data[sb->size] = '-';
    sb->size += 1;
    n = -n;
  }

  while (n > 0) {
    sb->data[sb->size] = (n % 10) + '0';
    sb->size += 1;
    n /= 10;
  }

  return OK;
}

RES sb_push_unsigned(Arena *a, StringBuilder *sb, u64 n) {
  u64 size = 0;
  for (u64 m = n; m != 0; m /= 10) {
    size += 1;
  }

  TRY(sb_grow(a, sb, sb->size + size));

  while (n > 0) {
    sb->data[sb->size] = (n % 10) + '0';
    sb->size += 1;
    n /= 10;
  }

  return OK;
}

RES sb_push_float(Arena *a, StringBuilder *sb, f64 n) {
  int size = snprintf(NULL, 0, "%f", n);
  TRY(sb_grow(a, sb, sb->size + size));

  snprintf(&sb->data[sb->size], size, "%f", n);
  sb->size += size;

  return OK;
}

RES sb_pushf(Arena *a, StringBuilder *sb, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  RES res = sb_pushfv(a, sb, fmt, ap);
  va_end(ap);
  return res;
}

RES sb_pushfv(Arena *a, StringBuilder *sb, const char *fmt_cstr, va_list ap) {
  StringView fmt = sv_from_cstr(fmt_cstr);

  if (sv_chop_left_s(&fmt, SV("%%"))) {
    TRY(sb_push_char(a, sb, '%'));
  } else if (sv_chop_left_s(&fmt, SV("%s"))) {
    StringView s = va_arg(ap, StringView);
    TRY(sb_push_sv(a, sb, s));
  } else if (sv_chop_left_s(&fmt, SV("%c"))) {
    char c = va_arg(ap, int);
    TRY(sb_push_char(a, sb, c));
  } else if (sv_chop_left_s(&fmt, SV("%d"))) {
    i64 c = va_arg(ap, i64);
    TRY(sb_push_int(a, sb, c));
  } else if (sv_chop_left_s(&fmt, SV("%u"))) {
    u64 c = va_arg(ap, u64);
    TRY(sb_push_unsigned(a, sb, c));
  } else if (sv_chop_left_s(&fmt, SV("%f"))) {
    f64 c = va_arg(ap, f64);
    TRY(sb_push_float(a, sb, c));
  }

  return OK;
}
