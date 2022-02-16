# tdups2checksum

Test Drive Unlimited (PS2) uses a checksum for its save games, storing it in the first 4 bytes of the file. As a part of trying to recover a corrupted save, I reverse-engineered this checksum algorithm and reimplemented it in C. While it didn't end up being useful in my situation, I figured I would post the algorithm regardless.

## How to use

Compile with any major C compiler like so:

```
gcc tduchecksum.c -o tduchecksum
```

Then run the resulting program on a Test Drive Unlimited save file:

```
./tduchecksum "BASLUS-21490 TDU"
```

The checksum is output as an unsigned 32-bit integer.

## Limitations

This code assumes a little-endian architecture (e.g. x86, most ARM). If you're running on a big-endian architecture (e.g. PowerPC) you will likely receive incorrect results.
