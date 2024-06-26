#include <string.h>
#include <ctype.h>

#include <flo.h>
#include <flo/mem.h>

#include <flo/sv.h>

StringView sv_from_parts(const char *data, u64 size) {
  return (StringView){data, size};
}

StringView sv_from_cstr(const char *cstr) {
  return sv_from_parts(cstr, strlen(cstr));
}

char *sv_to_cstr(Arena *a, StringView sv) {
  char *res = arena_allocz(a, sv.size + 1);
  if (sv.size > 0) {
    memcpy(res, sv.data, sv.size);
  }
  return res;
}

StringView sv_chop_left(StringView *sv, u64 len) {
  if (len > sv->size) {
    len = sv->size;
  }

  StringView res = { sv->data, len };
  sv->data += len;
  sv->size -= len;

  return res;
}

StringView sv_chop_left_while(StringView *sv, bool (*predicate)(char)) {
  u64 i = 0;
  while (i < sv->size && predicate(sv->data[i])) {
    i += 1;
  }

  return sv_chop_left(sv, i);
}

bool sv_chop_left_s(StringView *sv, StringView prefix) {
  if (sv_starts_with(*sv, prefix)) {
    (void)sv_chop_left(sv, prefix.size);
    return true;
  }

  return false;
}

StringView sv_chop_right(StringView *sv, u64 len) {
  if (len > sv->size) {
    len = sv->size;
  }

  StringView res = { sv->data + sv->size - len, sv->size - len };
  sv->size -= len;

  return res;
}

StringView sv_chop_right_while(StringView *sv, bool (*predicate)(char)) {
  u64 i = 0;
  while (i < sv->size && predicate(sv->data[sv->size - i - 1])) {
    i += 1;
  }

  return sv_chop_right(sv, i);
}

bool sv_chop_right_s(StringView *sv, StringView prefix) {
  if (sv_ends_with(*sv, prefix)) {
    (void)sv_chop_right(sv, prefix.size);
    return true;
  }

  return false;
}

StringView sv_chop_to(StringView *sv, StringView sep) {
  StringView res = { sv->data, 0 };

  while (sv->size > 0 && !sv_starts_with(*sv, sep)) {
    res.size += 1;
    sv->size -= 1;
    sv->data += 1;
  }

  if (sv_starts_with(*sv, sep)) {
    sv_chop_left(sv, sep.size);
  }

  return res;
}

static bool isspace_(char c) {
  return isspace(c);
}

StringView sv_trim_left(StringView sv) {
  (void)sv_chop_left_while(&sv, isspace_);
  return sv;
}

StringView sv_trim_right(StringView sv) {
  (void)sv_chop_right_while(&sv, isspace_);
  return sv;
}

StringView sv_trim(StringView sv) {
  (void)sv_chop_left_while(&sv, isspace_);
  (void)sv_chop_right_while(&sv, isspace_);
  return sv;
}

bool sv_equal(StringView a, StringView b) {
  if (a.size != b.size) {
    return false;
  }

  for (u64 i = 0; i < a.size; i += 1) {
    if (a.data[i] != b.data[i]) {
      return false;
    }
  }

  return true;
}

bool sv_equal_ignorecase(StringView a, StringView b) {
  if (a.size != b.size) {
    return false;
  }

  for (u64 i = 0; i < a.size; i += 1) {
    if (tolower(a.data[i]) != tolower(b.data[i])) {
      return false;
    }
  }

  return true;
}

bool sv_starts_with(StringView sv, StringView prefix) {
  if (prefix.size > sv.size) {
    return false;
  }

  for (u64 i = 0; i < prefix.size; i += 1) {
    if (sv.data[i] != prefix.data[i]) {
      return false;
    }
  }

  return true;
}

bool sv_ends_with(StringView sv, StringView suffix) {
  if (suffix.size < sv.size) {
    return false;
  }

  for (u64 i = 0; i < suffix.size; i += 1) {
    if (sv.data[sv.size - i - 1] != suffix.data[sv.size - i - 1]) {
      return false;
    }
  }

  return true;
}

u64 sv_to_u64(StringView sv) {
  u64 res = 0;
  for (u64 i = 0; i < sv.size && isdigit(sv.data[i]); i += 1) {
    res = res * 10 + (sv.data[i] - '0');
  }

  return res;
}

i64 sv_to_i64(StringView sv) {
  bool negative = false;

  if (sv_chop_left_s(&sv, SV("-"))) {
    negative = true;
  }

  u64 res = sv_to_u64(sv);
  return negative ? -res : res;
}

StringView sv_file_name(StringView sv) {
  i64 last = -1;
  for (u64 i = 0; i < sv.size; i += 1) {
    if (sv.data[i] == '/') {
      last = i;
    }
  }

  if (last >= 0) {
    return sv_from_parts(sv.data + last + 1, sv.size - last - 1);
  }

  return sv;
}

StringView sv_file_ext(StringView sv) {
  StringView file_name = sv_file_name(sv);

  i64 last = -1;
  for (u64 i = 0; i < file_name.size; i += 1) {
    if (file_name.data[i] == '.') {
      last = i;
    }
  }

  if (last > 0) {
    return sv_from_parts(file_name.data + last + 1, file_name.size - last - 1);
  }

  return (StringView){0};
}
