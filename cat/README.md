# Implementation of `cat` GNU/Linux utility.

The cat utility in GNU/Linux (short for "concatenate") is one of the most basic and frequently used commands. It is primarily used to display the content of files, but it can also be used for a variety of other purposes such as combining files, creating new files, and more. Below is a comprehensive guide to the `cat` command.

## Basic Syntax:
```bash
cat [OPTION] [FILE...]
```

Where:
* `OPTION` refers to various options you can use with cat to modify its behavior.
* `FILE` refers to the files you want to work with.

## Usage:
### Displaying File Contents
```bash
cat filename.txt
```

### Concatenating Files
```bash
cat file1.txt file2.txt
```

### Displaying Line Numbers
```bash
cat -n filename.txt
```

### Suppressing Blank Lines
```bash
cat -b filename.txt
```

### Showing Non-printing Characters
```bash
cat -v filename.txt
```

### Displaying Tabs as ^I and Newlines as $
```bash
cat -TE filename.txt
```

### Suppress repeated empty lines
```bash
cat -s filename.txt
```
