/**
 * @Author: S. Sharma <silentcat>
 * @Date:   2020-01-02T15:01:26-06:00
 * @Email:  silentcat@protonmail.com
 * @Last modified by:   silentcat
 * @Last modified time: 2020-01-02T17:36:04-06:00
 */

/**
 * Task: Search for files given a search directory and query.
 * Solution: Divide files into two types, regular files and directories
 * (ignore other types of files for now). If the file or directory matches
 * the query, print it out. Otherwise, if it's a directory, place all the files
 * it contains in a queue.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <errno.h>

#define print_err() fprintf(stderr, "Error: %s\n", strerror(errno))

struct linked_list
{
  char *path;
  struct linked_list *next;
};

typedef struct queue
{
  struct linked_list *first, *last;
} QUEUE;

#define DIRECT 0
#define FILE 1

QUEUE *init_queue(void);
void enqueue_file(QUEUE *q, char *path);
char *dequeue_file(QUEUE *q);

int determine_type(char *path);
void read_dir(QUEUE *q, char *dir_name);
int is_match(char *path, const char *query);
void search(char *path, const char *query);

QUEUE *init_queue()
{
  QUEUE *q = calloc(1, sizeof(QUEUE));
  if (!q) {
    print_err();
    return NULL;
  }
  q->first = NULL;
  q->last = NULL;
  return q;
}

void enqueue_file(QUEUE *q, char *path)
{
  if (!q->first) {
    q->first = calloc(1, sizeof(struct linked_list));
    if (!q->first) {
      print_err();
      return;
    }
    q->first->path = path;
    q->first->next = NULL;
    q->last = q->first;
  } else {
    struct linked_list *n = calloc(1, sizeof(struct linked_list));
    if (!n) {
      print_err();
      return;
    }
    n->path = path;
    n->next = NULL;
    q->last->next = n;
    q->last = q->last->next;
  }
}

char *dequeue_file(QUEUE *q)
{
  if (!q->first)
    return NULL;
  char *path = q->first->path;
  struct linked_list *next = q->first->next;
  free(q->first);
  q->first = next;
  return path;
}

int determine_type(char *path)
{
  struct stat buf;
  stat(path, &buf);
  mode_t mode = buf.st_mode;
  return ((mode & S_IFMT) == S_IFDIR) ? DIRECT
            : (((mode & S_IFMT) == S_IFREG) ? FILE : -1);
}

void read_dir(QUEUE *q, char *dir_name)
{
  struct dirent *f = NULL;
  DIR *d = opendir(dir_name);
  if (!d) {
    print_err();
    return;
  }
  while ((f = readdir(d))) {
    if (!strcmp(f->d_name, ".") || !strcmp(f->d_name, ".."))
      continue;
    char *path = calloc(1, strlen(f->d_name) + strlen(dir_name) + 2);
    if (!path) {
      print_err();
      closedir(d);
      return;
    }
    strncpy(path, dir_name, strlen(dir_name));
    strncat(path, "/", 1);
    strncat(path, f->d_name, strlen(f->d_name));
    enqueue_file(q, path);
  }
  closedir(d);
}

int is_match(char *path, const char *query)
{
  int i = 0;
  for (i = strlen(path)-1; i > 0; i--)
    if (path[i] == '/')
      break;
  return strncmp(query, path + i + 1, strlen(query));
}

void search(char *dir, const char *query)
{
  QUEUE *q = init_queue();
  if (!q)
    return;
  enqueue_file(q, dir);
  while (q->first) {
    char *file = dequeue_file(q);
    int type = determine_type(file);
    if (type == DIRECT)
      read_dir(q, file);
    else if (type == FILE) {
      if (!is_match(file, query))
        printf("%s\n", file);
    }
    free(file);
  }
}


int main(int argc, char *argv[])
{
  if (argc < 3) {
    fprintf(stderr, "Usage: %s [search dir] [search query]\n", argv[0]);
    return 1;
  }
  search(strdup(argv[1]), argv[2]);
  return 0;
}
