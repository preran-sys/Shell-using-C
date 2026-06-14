# fgets()

### Understanding [Part 1]

```c++
char *
     fgets(char * restrict str, int size, FILE * restrict stream);
```
DESCRIPTION

The fgets() function reads **at most** one less than the number of characters
specified by size from the given stream and stores them in the string
str.  Reading stops when a newline character is found, at end-of-file or
error.  The newline, if any, is retained.  If any characters are read and
there is no error, a `\0' character is appended to end the string.


> reads **at most** one less than the number of characters
specified by size from the given stream

For instance:

INPUT STREAM
────────────────────────────────
ABCDEFGH\n
────────────────────────────────

```c++
#define SIZE 8

char str[SIZE];

fgets(str, sizeof(str), stdin);
```

* `size` = 8
* `stream:` ABCDEFGH\n
* one less than the # characters specified by size = 7
* store `ABCDEFG` in `str`
* Reading does not stop: `\n` or `EOF` not found yet
* `\0` is appended to `ABCDEFG`, `buffer` after `fgets` `{'A','B','C','D','E','F','G','\0'}`
* `stream` after `fgets` `H\n`

**Question(s):**
* Description says Reading stops when a newline character is found, at end-of-file or
  error. However, we never find it. How did it successfully return a pointer to char?

**Answer**:

Clarification:
* "reads at most one less than size"
* **means** `fgets will never write more than size-1 bytes into str`
* **NOT**: `fgets` will only read that much from the stream

Think of `fgets` like this:

It tries to fill your buffer,
BUT it is forced to stop if ANY of these happen:

1. buffer is full (size - 1 reached)
2. newline appears
3. EOF appears

Whichever happens first wins.

**Guarantee:** `fgets` will never overflow your buffer

> Even without finding \n or EOF, fgets() returns successfully because:
> 
success means `fgets` copied at least one character safely

RETURN VALUES

Upon successful completion, fgets() and gets() return a pointer to the
string.  If end-of-file occurs before any characters are read, they
return NULL and the buffer contents remain unchanged.  If an error
occurs, they return NULL and the buffer contents are indeterminate.  The
fgets() and gets() functions do not distinguish between end-of-file and
error, and callers must use feof(3) and ferror(3) to determine which
occurred.