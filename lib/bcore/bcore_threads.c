/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_threads.h"
#include "bcore_control.h"

void bcore_mutex_init( bcore_mutex_t* o )
{
    int ern = pthread_mutex_init( o, NULL );
    if( ern ) ERR( "function returned error %i", ern );
}

void bcore_mutex_down( bcore_mutex_t* o )
{
    int ern = pthread_mutex_destroy( o );
    if( ern ) ERR( "function returned error %i", ern );
}

void bcore_mutex_lock( bcore_mutex_t* o )
{
    int ern = pthread_mutex_lock( o );
    if( ern ) ERR( "function returned error %i", ern );
}

void bcore_mutex_unlock( bcore_mutex_t* o )
{
    int ern = pthread_mutex_unlock( o );
    if( ern ) ERR( "function returned error %i", ern );
}

void bcore_once( bcore_once_t* flag, void (*func)() )
{
    int ern = pthread_once( flag, func );
    if( ern ) ERR( "function returned error %i", ern );
}

vd_t bcore_threads_signal( tp_t target, tp_t signal, vd_t object )
{
    return NULL;
}

