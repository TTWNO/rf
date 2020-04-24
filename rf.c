#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

int files_in_dir(DIR* dp, struct dirent* entry, bool hidden_files){
  int files = 0;
  while ((entry = readdir(dp))){
    if (hidden_files){
      if (!strcmp(entry->d_name, "..") == 0 &&
          !strcmp(entry->d_name, ".") == 0){
        files++;
      }
    } else {
      if (entry->d_name[0] != '.'){
        files++;
      }
    }
  }
  return files;
}

void print_dir(DIR* dp, struct dirent* entry, int pe, bool hidden_files){
  int files = 0;
  while ((entry = readdir(dp))){
    if (hidden_files){
      if (!strcmp(entry->d_name, "..") == 0 &&
          !strcmp(entry->d_name, ".") == 0){
        files++;
      }
    } else {
      if (entry->d_name[0] != '.'){
        files++;
      }
    }
    if (files == pe){
      printf("%s\n", entry->d_name);
      return;
    }
  }
}

int main(int argc, char** argv){
  DIR* dp;
  struct dirent* entry;
  bool hidden_files = false;

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

  int files = files_in_dir(dp, entry, hidden_files); 
  if (files == 0){
    fprintf(stderr, "directory is an empty directory.\n");
    return -1;
  }
  rewinddir(dp);
  print_dir(dp, entry, 1+rand()/((RAND_MAX + 1u)/files), hidden_files);

  closedir(dp);
  return 0;
}
