#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
#include "kernel/syscall.h"
#include "user/user.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}


void find(char *path, char *name) {
	int fd;
	struct dirent de;
	struct stat st;
	char buf[512], *p;

	if ((fd = open(path, O_RDONLY)) < 0) {
		fprintf(2, "find: cannot open %s\n", path);
		return;
	}

	if (fstat(fd, &st) < 0) {
		fprintf(2, "find: cannot stat %s\n", path);
		close(fd);
		return;
	}

	switch (st.type) {
		case T_DEVICE:
		case T_FILE:
			memset(name + strlen(name), 0, DIRSIZ - strlen(name));
			if (strcmp(fmtname(path), name) == 0) {
				printf("%s\n", path);
			}
			break;
		case T_DIR:
			if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
				printf("find: path too long\n");
				break;
			}
			strcpy(buf, path);
			p = buf + strlen(buf);
			*p++ = '/';
			while (read(fd, &de, sizeof(de)) == sizeof(de)) {
				if (de.inum == 0) {
					continue;
				}
				memmove(p, de.name, DIRSIZ);
				p[DIRSIZ] = 0;
				if (stat(buf, &st) < 0) {
					fprintf(2, "find: cannot stat %s\n", buf);
					continue;
				}
				if (st.type == T_DIR) {
					if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) {
						continue;
					}
					find(buf, name);
				} else if (st.type == T_FILE) {
					if (strcmp(de.name, name) == 0) {
						printf("%s\n", buf);
					}
				}
			}
			break;
	}
	close(fd);
}

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("Usage: find <path> <name>\n");
		exit(1);
	}

	find(argv[1], argv[2]);
	exit(0);
}