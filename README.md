# rf
Random File: Return a random file in a given directory

### Usage:

`rf [-a|--all] [DIRECTORY]`

By default, `rf` will select a random file from within the current directory, not including hidden files.

Run with the -a, or --all option to allow hidden files to be randomly selected as well.

Add a directory on the end of the command to run in any directory other than the current one.

## TODO:

1. Add man page.
2. Allow for extra options like:
  1. `-r` to select a random file recursively.
  2. `-f` to select only a random file, not a directory.
  3. `-d` to select only a random directory, not a file.

## Done:

1. Randomly select a file in a directory *EVEN IF* the file contains a space or a newline.
2. Add `-a` to allow for accessing hidden files.
