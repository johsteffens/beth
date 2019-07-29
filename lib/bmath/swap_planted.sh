#! /bin/bash
# swaps planted with backup

mv bmath_planted.h.backup bmath_planted.h.swap
mv bmath_planted.h        bmath_planted.h.backup
mv bmath_planted.h.swap   bmath_planted.h

mv bmath_planted.c.backup bmath_planted.c.swap
mv bmath_planted.c        bmath_planted.c.backup
mv bmath_planted.c.swap   bmath_planted.c

