#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>

static int file_counter;
static char** names;

struct stat st;

void add_to_num_of_files(struct dirent* entry, int fi)
{
  file_counter++;
}

void add_file_to_list(struct dirent* entry, int fi)
{
  strcpy(names[fi], entry->d_name);
}


int files_in_dir(DIR* dp, struct dirent* entry, bool hidden_files, void (*func)(struct dirent*, int)){
  int files = 0;
  while ((entry = readdir(dp))){
    if (entry->d_name[0] != '.' ||
        (hidden_files &&
        !strcmp(entry->d_name, "..") == 0 &&
        !strcmp(entry->d_name, ".") == 0)){
      (*func)(entry, files);
      ++files;
    }
  }
  return files;
}

int main(int argc, char** argv){
  DIR* dp;
  struct dirent* entry;
  bool hidden_files = false;

  file_counter = 0;

  /* Best random thing I could come up with which is cross compileable 
   * Should be really random, even if it overflows :)*/
  srand(clock() * time(0));

  int i = 0;
  for (; i < argc; i++){
    if (strcmp(argv[i], "-a") == 0 ||
        strcmp(argv[i], "--all") == 0){
      hidden_files = true;
    }
  }
  if ((hidden_files && argc > 2) ||
      (!hidden_files && argc > 1)){
    dp = opendir(argv[argc-1]);
  } else {
    dp = opendir(".");
  }

  if (dp == NULL){
    perror("opendir");
    return -1;
  }

  files_in_dir(dp, entry, hidden_files, add_to_num_of_files); 
  if (file_counter == 0){
    fprintf(stderr, "directory is an empty directory.\n");
    return -1;
  }
  rewinddir(dp);
  names = malloc(sizeof(char*) * file_counter);
  for (i = 0; i < file_counter; ++i)
  {
    names[i] = malloc(sizeof(char) * 256);
  }
  files_in_dir(dp, entry, hidden_files, add_file_to_list);
  int rando = rand()/((RAND_MAX + 1u)/file_counter);
  printf("%s\n", names[rando]);

  closedir(dp);
  return 0;
}
