#ifndef BASE_FS_H
#define BASE_FS_H

#include <flo.h>
#include <flo/mem.h>
#include <flo/sb.h>
#include <flo/sv.h>

RES fs_read_entire_file(Arena *, StringBuilder *to, StringView filename);

#endif // BASE_FS_H
