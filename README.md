# rf
Random File: Return a random file in a given directory

### Usage:

`rf [-afd] [DIRECTORY]`

By default, `rf` will select a random file from within the current directory, not including hidden files.

Options:
* Run with the -a option to allow hidden files (starting with ".") to be randomly selected as well.
* Run with -f to select a random file, not a directory.
* Run with -d to select a random directory, not a file.

Add a directory on the end of the command to run in any directory other than the current one.

## TODO:

1. Add man page.
2. Allow for extra options like:
  1. `-r` to select a random file recursively.
3. Don't depend on glibc (GNU C). Currently using getopt.

## Done:

1. Randomly select a file in a directory *EVEN IF* the file contains a space or a newline.
2. Add `-a` to allow for accessing hidden files.
3. Use `-f` to allow for searching of files only.
4. Use `-d` to allow for searching of directories only.
