#ifndef BASE_SV_H
#define BASE_SV_H

#include <flo.h>

typedef struct {
  const char *data;
  u64 size;
} StringView;

#define SV(str_lit) ((StringView){ (str_lit), sizeof(str_lit) - 1 })

// printf macros for StringView
// usage: printf("name: "SV_FMT"\n", SV_ARG(name));
#define SV_FMT "%.*s"
#define SV_ARG(sv) (int)(sv).size, (sv).data

StringView sv_from_parts(const char *data, u64 size);
StringView sv_from_cstr(const char *cstr);

StringView sv_trim_left(StringView);
StringView sv_trim_right(StringView);
StringView sv_trim(StringView);

StringView sv_chop_left(StringView *, u64 len);
StringView sv_chop_left_while(StringView *, bool (*predicate)(char));
bool sv_chop_left_s(StringView *, StringView prefix);

StringView sv_chop_right(StringView *, u64 len);
StringView sv_chop_right_while(StringView *, bool (*predicate)(char));
bool sv_chop_right_s(StringView *, StringView suffix);

StringView sv_chop_to(StringView *, StringView sep);

bool sv_equal(StringView, StringView);
bool sv_equal_ignorecase(StringView, StringView);

bool sv_starts_with(StringView, StringView prefix);
bool sv_ends_with(StringView, StringView suffix);

i64 sv_to_i64(StringView);
u64 sv_to_u64(StringView);

#endif // BASE_SV_H
