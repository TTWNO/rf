#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define FILE_ONLY_MASK 0x1
#define DIRECTORY_ONLY_MASK 0x2
#define RECURSIVE_MASK 0x4
#define HIDDEN_FILE_MASK 0x8
#define STDIN_MASK 0x10

uint8_t options = 0x00;

static int file_counter;
static char** names;

struct stat* st;

void add_to_num_of_files(struct dirent* entry, int fi)
{
  file_counter++;
}

void add_file_to_list(struct dirent* entry, int fi)
{
  strcpy(names[fi], entry->d_name);
}

/* TODO: this is not the prettiest thing I have ever written. Work on making it easier to maintain */
int files_in_dir(DIR* dp, struct dirent* entry, void (*func)(struct dirent*, int)){
  int files = 0;
  while ((entry = readdir(dp))){
    stat(entry->d_name, st);
    if (entry->d_name[0] != '.' ||
        (options & HIDDEN_FILE_MASK &&
        !strcmp(entry->d_name, "..") == 0 &&
        !strcmp(entry->d_name, ".") == 0)){
      if ((options & FILE_ONLY_MASK &&
          S_ISREG(st->st_mode)) ||
          (options & DIRECTORY_ONLY_MASK &&
          S_ISDIR(st->st_mode)) ||
          (options ^ DIRECTORY_ONLY_MASK && options ^ FILE_ONLY_MASK)) {
        (*func)(entry, files);
        ++files;
      }
    }
  }
  return files;
}

int main(int argc, char** argv){
  DIR* dp;
  struct dirent* entry;
  int opt, opts = 0;
  int i;
  st = malloc(sizeof(struct stat));
  
  file_counter = 0;

  /* Best random thing I could come up with which is cross compileable 
   * Should be really random, even if it overflows :)*/
  srand(clock() * time(0));
  while ((opt = getopt(argc, argv, "adfri")) != -1) {
    switch (opt) {
      case 'a':
        options |= HIDDEN_FILE_MASK;
        opts++;
        break;
      case 'd':
        options |= DIRECTORY_ONLY_MASK;
        opts++;
        break;
      case 'f':
        options |= FILE_ONLY_MASK;
        opts++;
        break;
      case 'r':
        options |= RECURSIVE_MASK;
        opts++;
        break;
      case 'i':
        options |= STDIN_MASK;
        opts++;
        break;
      default:
        printf("Unsupported option \"-%c\".\n", opt);
        exit(1);
    }
  }

  // stamp out invalid options
  if (options & FILE_ONLY_MASK && options & DIRECTORY_ONLY_MASK) {
    printf("-f and -d are mutually exclusive.\n");
    exit(1);
  }
  if (options & STDIN_MASK && options != STDIN_MASK) {
    printf("-i is not compatible with any other switch.\n");
  }

  if (strncmp("-", argv[argc-1], 1) == 0 || argc < 2) {
    dp = opendir(".");
  } else {
    dp = opendir(argv[argc-1]);
  }

  if (dp == NULL){
    perror("opendir");
    exit(1);
  }

  files_in_dir(dp, entry, add_to_num_of_files); 
  if (file_counter == 0){
    if (options & FILE_ONLY_MASK || options & DIRECTORY_ONLY_MASK) {
      fprintf(stderr, "no results.\n");
    } else {
      fprintf(stderr, "directory is an empty directory.\n");
    }
    exit(1);
  }
  rewinddir(dp);
  names = malloc(sizeof(char*) * file_counter);
  for (i = 0; i < file_counter; ++i)
  {
    names[i] = malloc(sizeof(char) * 256);
  }
  files_in_dir(dp, entry, add_file_to_list);
  int rando = rand()/((RAND_MAX + 1u)/file_counter);
  printf("%s\n", names[rando]);

  closedir(dp);
  return 0;
}
