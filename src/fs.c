#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include <flo.h>
#include <flo/mem.h>
#include <flo/sb.h>
#include <flo/sv.h>

#include <flo/fs.h>

#define BUFSIZE 4096

RES fs_read_entire_file(Arena *a, StringBuilder *to, StringView filename) {
  RES res = OK;
  char *cfilename = sv_to_cstr(a, filename);

  int fd = open(cfilename, O_RDONLY);
  if (fd < 0) {
    fprintf(stderr, "Could not open %s: %s\n", cfilename, strerror(errno));
    res = ERR; goto defer;
  }

  struct stat st;
  int r = fstat(fd, &st);
  if (r < 0) {
    fprintf(stderr, "Could not stat %s: %s\n", cfilename, strerror(errno));
    res = ERR; goto defer;
  }

  char buf[BUFSIZE + 1] = {0};
  while ((r = read(fd, buf, BUFSIZE)) > 0) {
    buf[r] = '\0';

    if (!sb_push_sv(a, to, sv_from_parts(buf, r))) {
      res = ERR; goto defer;
    }
  }

  if (r < 0) {
    fprintf(stderr, "Could not read from %s: %s\n", cfilename, strerror(errno));
    res = ERR; goto defer;
  }

defer:
  close(fd);
  return res;
}
