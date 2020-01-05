/**
 * @Author: S. Sharma <silentcat>
 * @Date:   2020-01-04T16:58:18-06:00
 * @Email:  silentcat@protonmail.com
 * @Last modified by:   silentcat
 * @Last modified time: 2020-01-04T18:06:36-06:00
 */

/**
 * Task: Create a simple grep program that can find expressions in files.
 * Print these expressions out to standard output in colored format with line
 * numbers.
 * Solution: Read text from the file, separate each token, match it with the
 * string, and then print the line in color.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define print_err() fprintf(stderr, "Error: %s\n", strerror(errno))

char *read_file(const char *path);
void read_tokens(char *buffer, char *match);
int is_match(char *token, char *match);
void write_line(char *token, char *line, int n, int pos);


char *read_file(const char *path)
{
  char *buffer = NULL;
  FILE *fp = fopen(path, "r");
  if (!fp) {
    print_err();
    return NULL;
  }

  fseek(fp, 0, SEEK_END);
  int size = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  buffer = calloc(1, size+1);
  if (!buffer) {
    print_err();
    fclose(fp);
    return NULL;
  }
  if (fread(buffer, 1, size, fp) < 0) {
    print_err();
    fclose(fp);
    free(buffer);
    return NULL;
  }
  fclose(fp);
  return buffer;
}

void read_tokens(char *buffer, char *match)
{
  int n = 1;
  char *cpy = strdup(buffer), *line, *token;
  while ((line = strsep(&cpy, "\n"))) {
    int pos = 0;
    char *line_cpy = strdup(line);
    while ((token = strsep(&line_cpy, " "))) {
      if (is_match(token, match)) {
        write_line(token, line, n, pos);
        break;
      }
      pos += strlen(token) + 1;
    }
    while ((token = strsep(&line_cpy, " ")));
    free(line_cpy);
    ++n;
  }
  free(cpy);
}

int is_match(char *token, char *match)
{
  return !strncmp(match, token, strlen(match));
}

void write_line(char *token, char *line, int n, int pos)
{
  char *str = calloc(1, 2*strlen(line)+1);
  if (!str) {
    print_err();
    return;
  }
  strncpy(str, line, pos);
  strncat(str, "\033[0;31m", 7); // Red
  strncat(str, line + pos, strlen(token));
  strncat(str, "\033[0m", 5); // No color
  strncat(str, line + pos + strlen(token), strlen(line)-strlen(token)-pos);
  printf("%d: %s\n", n, str);
  free(str);
}

int main(int argc, char *argv[])
{
  if (argc < 3) {
    fprintf(stderr, "Usage: %s [match] [filename]\n", argv[0]);
    return 1;
  }

  for (int i = 2; i < argc; i++) {
    printf("\033[1;33m%s\033[0m \n", argv[i]); // Yellow, string, no color 
    char *buffer = read_file(argv[i]);
    if (buffer) {
      read_tokens(buffer, argv[1]);
      free(buffer);
    }
  }
  return 0;
}
