/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#ifndef BCORE_NAME_H
#define BCORE_NAME_H

#include "bcore_st.h"
#include "bcore_hmap.h"

typedef struct bcore_name_s
{
    tp_t name_space;
    sd_t name;
} hnode_s;

vd_t bcore_name_signal( tp_t target, tp_t signal, vd_t object );

#endif // BCORE_NAME_H
