/** Copyright 2018 Johannes Bernhard Steffens
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include "bclos_machine.h"
#include "bclos_quicktypes.h"
#include "bcore_quicktypes.h"
#include "bcore_spect_inst.h"
#include "bcore_spect_array.h"
#include "bcore_life.h"
#include "bcore_trait.h"
#include "bcore_txt_ml.h"

/**********************************************************************************************************************/

/// 64 bit instruction
#define TYPEOF_bclos_minst_s typeof( "bclos_minst_s" )
typedef struct bclos_minst_s
{
    union
    {
        u3_t u3;
        f3_t f3;
        struct
        {
            u2_t oc; // opcode
            u0_t o0; // operants..
            u0_t o1;
            u0_t o2;
            u0_t o3;
        };
    };
} bclos_minst_s;

DEFINE_FUNCTIONS_OBJ_INST( bclos_minst_s )
static sc_t bclos_minst_s_def_g = "bclos_minst_s = bcore_inst { u2_t inst; u0_t op0; u0_t op1; u0_t op2; u0_t op3; }";
DEFINE_CREATE_SELF( bclos_minst_s, bclos_minst_s_def_g )

bclos_minst_s bclos_minst_s_create0( u2_t oc )                            { return (bclos_minst_s){ .oc=oc, .o0 =  0, .o1 =  0, .o2 =  0, .o3 =  0 }; }
bclos_minst_s bclos_minst_s_create1( u2_t oc, u0_t o0 )                   { return (bclos_minst_s){ .oc=oc, .o0 = o0, .o1 =  0, .o2 =  0, .o3 =  0 }; }
bclos_minst_s bclos_minst_s_create2( u2_t oc, u0_t o0, u0_t o1 )          { return (bclos_minst_s){ .oc=oc, .o0 = o0, .o1 = o1, .o2 =  0, .o3 =  0 }; }
bclos_minst_s bclos_minst_s_create3( u2_t oc, u0_t o0, u0_t o1, u0_t o2 ) { return (bclos_minst_s){ .oc=oc, .o0 = o0, .o1 = o1, .o2 = o2, .o3 =  0 }; }
bclos_minst_s bclos_minst_s_u3(      u3_t v )                             { return (bclos_minst_s){ .u3 = v }; }
bclos_minst_s bclos_minst_s_f3(      f3_t v )                             { return (bclos_minst_s){ .f3 = v }; }

/**********************************************************************************************************************/

/// machine code
#define TYPEOF_bclos_mcode_s typeof( "bclos_mcode_s" )
typedef struct bclos_mcode_s
{
    aware_t _;
    bcore_arr_u3_s arr;
} bclos_mcode_s;

DEFINE_FUNCTIONS_OBJ_INST( bclos_mcode_s )
static sc_t bclos_mcode_s_def_g = "bclos_mcode_s = bcore_inst { aware_t _; bcore_arr_u3_s arr; }";
DEFINE_CREATE_SELF( bclos_mcode_s, bclos_mcode_s_def_g )

void bclos_mcode_s_push_minst( bclos_mcode_s* o, bclos_minst_s v ) { bcore_arr_u3_s_push( &o->arr, v.u3 ); }
void bclos_mcode_s_push_u3( bclos_mcode_s* o, u3_t v ) { bclos_mcode_s_push_minst( o, bclos_minst_s_u3( v ) ); }
void bclos_mcode_s_push_f3( bclos_mcode_s* o, f3_t v ) { bclos_mcode_s_push_minst( o, bclos_minst_s_f3( v ) ); }
void bclos_mcode_s_push_op0( bclos_mcode_s* o, u2_t oc ) { bclos_mcode_s_push_minst( o, bclos_minst_s_create0( oc ) ); }
void bclos_mcode_s_push_op1( bclos_mcode_s* o, u2_t oc, u0_t o0 ) { bclos_mcode_s_push_minst( o, bclos_minst_s_create1( oc, o0 ) ); }
void bclos_mcode_s_push_op2( bclos_mcode_s* o, u2_t oc, u0_t o0, u0_t o1 ) { bclos_mcode_s_push_minst( o, bclos_minst_s_create2( oc, o0, o1 ) ); }
void bclos_mcode_s_push_op3( bclos_mcode_s* o, u2_t oc, u0_t o0, u0_t o1, u0_t o2 ) { bclos_mcode_s_push_minst( o, bclos_minst_s_create3( oc, o0, o1, o2 ) ); }

/**********************************************************************************************************************/

/// cpu
#define TYPEOF_bclos_mcpu_s typeof( "bclos_mcpu_s" )
typedef struct bclos_mcpu_s
{
    aware_t _;

    /// general purpose registers
    u3_t r[ 32 ];

    /// instruction pointer
    u3_t ip;

    /// stack pointer
    u3_t sp;

} bclos_mcpu_s;

DEFINE_FUNCTIONS_OBJ_INST( bclos_mcpu_s )
static sc_t bclos_mcpu_s_def_g =

"bclos_mcpu_s = bcore_inst { "
    "aware_t _;"

    /// general purpose registers
    "u3_t r00; u3_t r01; u3_t r02; u3_t r03; u3_t r04; u3_t r05; u3_t r06; u3_t r07;"
    "u3_t r08; u3_t r09; u3_t r0a; u3_t r0b; u3_t r0c; u3_t r0d; u3_t r0e; u3_t r0f;"
    "u3_t r10; u3_t r11; u3_t r12; u3_t r13; u3_t r14; u3_t r15; u3_t r16; u3_t r17;"
    "u3_t r18; u3_t r19; u3_t r1a; u3_t r1b; u3_t r1c; u3_t r1d; u3_t r1e; u3_t r1f;"

    "u3_t ip;" // instruction pointer


    "u3_t sp;" // stack pointer
"}";

DEFINE_CREATE_SELF( bclos_mcpu_s, bclos_mcpu_s_def_g )

/**********************************************************************************************************************/

/** Naming scheme CMD_oooo:
 * CMD: principal command
 * o:   usage of operand
 *    * x: general purpose register
 *    * u: register as unsigned value
 *    * s: register as signed value
 *    * a: register as address value
 *    * c: embedded constant number (unsigned 8 bit)
 *    * d: embedded constant number (signed 8 bit)
 *    * C: embedded constant (from next op entry)
 *    * D: embedded signed constant (from next op entry)
 *    * A: embedded constant address number   (from next op entry)
 *    * I: IP register
 *    * S: SP register
 *    * F: flags register
 */
enum bclos_machine_inst_set
{
    NOOP = 0, // no operation

    /// constant -> register
    CP_Cx, // full constant
    CP_cx, // 8bit unsigned constant

    /// register -> register (exact copy)
    CP_xx,

    /// memory -> register
    CP_ax,
    CP_Ax,

    /// memory+signed_offset -> register
    CP_asx,
    CP_Asx,
    CP_adx,
    CP_Adx,

    /// register -> memory
    CP_xa,
    CP_xA,

    /// register -> memory+signed_offset
    CP_xas,
    CP_xAs,
    CP_xad,
    CP_xAd,

    /// register -> register (type conversion)
    CP_uf,
    CP_sf,
    CP_fs,

    /// special registers <-> register
    CP_Ix, // ip -> x
    CP_Sx, // sp -> x
    CP_xI, // x -> ip
    CP_xS, // x -> sp

    /// arithmetic
    AD_uuu,
    AD_fff,
    SB_uuu,
    SB_fff,
    ML_uuu,
    ML_sss,
    ML_fff,
    DV_uuu,
    DV_sss,
    DV_fff,
    MD_uuu, // modulo

    INC_u,
    DEC_u,

    /// logic
    AND_uuu,
     OR_uuu,
    XOR_uuu,
    NOT_uu,

    /// shift, rotation
    SR_uu, // logic 1 bit
    SR_ss, // arithmetic 1 bit
    SL_uu, // logic 1 bit

    SR_cuu, // logic c bits
    SR_css, // arithmetic c bits
    SL_cuu, // logic c bits

    SR_uuu, // logic u bits
    SR_sss, // arithmetic u bits
    SL_uuu, // logic u bits

    RR_uu,  // rotate right 1 bit
    RL_uu,  // rotate left 1 bit
    RR_cuu, // rotate right n bits
    RL_cuu, // rotate left n bits
    RR_uuu, // rotate right n bits
    RL_uuu, // rotate left n bits

    /// jump
    JP_C,
    JP_a,
    JPZR_uA,
    JPZR_fA,
    JPZR_ua,
    JPZR_fa,
    JPNZ_uA,
    JPNZ_fA,
    JPNZ_ua,
    JPNZ_fa,

    JPEQ_uCA,
    JPNE_uCA,
    JPLG_uCA,
    JPLG_sCA,
    JPLG_fCA,
    JPLE_uCA,
    JPLE_sCA,
    JPLE_fCA,
    JPSM_uCA,
    JPSM_sCA,
    JPSM_fCA,
    JPSE_uCA,
    JPSE_sCA,
    JPSE_fCA,
    JPEQ_uuA,
    JPNE_uuA,
    JPLG_uuA,
    JPLG_ssA,
    JPLG_ffA,
    JPLE_uuA,
    JPLE_ssA,
    JPLE_ffA,
    JPSM_uuA,
    JPSM_ssA,
    JPSM_ffA,
    JPSE_uuA,
    JPSE_ssA,
    JPSE_ffA,
    JPEQ_uua,
    JPNE_uua,
    JPLG_uua,
    JPLG_ssa,
    JPLG_ffa,
    JPLE_uua,
    JPLE_ssa,
    JPLE_ffa,
    JPSM_uua,
    JPSM_ssa,
    JPSM_ffa,
    JPSE_uua,
    JPSE_ssa,
    JPSE_ffa,

    /// subroutine: call, return
    CSR_A,
    CSR_a,
    RSR,

    /// push, pop
    PUS_x,
    PUS_a,
    PUS_I,
    PUS_S,
    POP_x,
    POP_a,
    POP_I,
};

void bclos_machine_run( const u3_t* code, sz_t code_size, u3_t* data, sz_t data_size )
{
    if( !code ) return;

    bclos_mcpu_s pu;
    bclos_mcpu_s_init( &pu );
    pu.sp = 0;

    u3_t* up = ( u3_t* )pu.r;
    s3_t* sp = ( s3_t* )pu.r;
    f3_t* fp = ( f3_t* )pu.r;

    while( pu.ip < code_size && pu.sp < data_size )
    {
        bclos_minst_s op = ( ( bclos_minst_s* )code )[ pu.ip++ ];
        switch( op.oc )
        {
            case NOOP : break;

            case CP_Cx: up[ op.o1 ] = code[ pu.ip++ ]; break;
            case CP_cx: up[ op.o1 ] = op.o0; break;
            case CP_xx: up[ op.o1 ] = up[ op.o0 ]; break;
            case CP_ax: up[ op.o1 ] = data[ up[ op.o0 ] ]; break;
            case CP_Ax: up[ op.o1 ] = data[ code[ pu.ip++ ] ]; break;

            /// memory+signed_offset -> register
            case CP_asx: up[ op.o2 ] = data[ up[ op.o0 ]     + up[ op.o1 ] ]; break;
            case CP_Asx: up[ op.o2 ] = data[ code[ pu.ip++ ] + up[ op.o1 ] ]; break;
            case CP_adx: up[ op.o2 ] = data[ up[ op.o0 ]     + ( s0_t )op.o1 ]; break;
            case CP_Adx: up[ op.o2 ] = data[ code[ pu.ip++ ] + ( s0_t )op.o1 ]; break;

            /// register -> memory
            case CP_xa: data[ up[ op.o1 ] ]     = up[ op.o0 ]; break;
            case CP_xA: data[ code[ pu.ip++ ] ] = up[ op.o0 ]; break;

            /// register -> memory+signed_offset
            case CP_xas: data[ up[ op.o1 ]     + up[ op.o2 ] ]   = up[ op.o0 ]; break;
            case CP_xAs: data[ code[ pu.ip++ ] + up[ op.o2 ] ]   = up[ op.o0 ]; break;
            case CP_xad: data[ up[ op.o1 ]     + ( s0_t )op.o2 ] = up[ op.o0 ]; break;
            case CP_xAd: data[ code[ pu.ip++ ] + ( s0_t )op.o2 ] = up[ op.o0 ]; break;

            /// register -> register (type conversion)
            case CP_uf: fp[ op.o1 ] =         pu.r[ op.o0 ]; break;
            case CP_sf: fp[ op.o1 ] = ( s3_t )pu.r[ op.o0 ]; break;
            case CP_fs: sp[ op.o1 ] = ( f3_t )pu.r[ op.o0 ]; break;

            /// special registers <-> register
            case CP_Ix: up[ op.o1 ] = pu.ip; break;
            case CP_Sx: up[ op.o1 ] = pu.sp; break;
            case CP_xI: pu.ip = up[ op.o0 ]; break;
            case CP_xS: pu.sp = up[ op.o0 ]; break;

            /// arithmetic
            case AD_uuu: up[ op.o2 ] = up[ op.o0 ] + up[ op.o1 ]; break;
            case AD_fff: fp[ op.o2 ] = fp[ op.o0 ] + fp[ op.o1 ]; break;
            case SB_uuu: up[ op.o2 ] = up[ op.o0 ] - up[ op.o1 ]; break;
            case SB_fff: fp[ op.o2 ] = fp[ op.o0 ] - fp[ op.o1 ]; break;
            case ML_uuu: up[ op.o2 ] = up[ op.o0 ] * up[ op.o1 ]; break;
            case ML_sss: sp[ op.o2 ] = sp[ op.o0 ] * sp[ op.o1 ]; break;
            case ML_fff: fp[ op.o2 ] = fp[ op.o0 ] * fp[ op.o1 ]; break;
            case DV_uuu: up[ op.o2 ] = up[ op.o0 ] / up[ op.o1 ]; break;
            case DV_sss: sp[ op.o2 ] = sp[ op.o0 ] / sp[ op.o1 ]; break;
            case DV_fff: fp[ op.o2 ] = fp[ op.o0 ] / fp[ op.o1 ]; break;
            case MD_uuu: up[ op.o2 ] = up[ op.o0 ] % up[ op.o1 ]; break;

            case INC_u: up[ op.o0 ]++; break;
            case DEC_u: up[ op.o0 ]--; break;

            /// logic
            case AND_uuu: up[ op.o2 ] = up[ op.o0 ] & up[ op.o1 ]; break;
            case  OR_uuu: up[ op.o2 ] = up[ op.o0 ] | up[ op.o1 ]; break;
            case XOR_uuu: up[ op.o2 ] = up[ op.o0 ] ^ up[ op.o1 ]; break;
            case NOT_uu : up[ op.o1 ] = ~up[ op.o0 ]; break;

            /// shift, rotation
            case SR_uu: up[ op.o1 ] = up[ op.o0 ] >> 1; break;
            case SR_ss: sp[ op.o1 ] = sp[ op.o0 ] >> 1; break;
            case SL_uu: up[ op.o1 ] = up[ op.o0 ] << 1; break;

            case SR_cuu: up[ op.o2 ] = up[ op.o1 ] >> op.o0; break;
            case SR_css: sp[ op.o2 ] = sp[ op.o1 ] >> ( s0_t )op.o0; break;
            case SL_cuu: up[ op.o2 ] = up[ op.o1 ] << op.o0; break;

            case SR_uuu: up[ op.o2 ] = up[ op.o1 ] >> up[ op.o0 ]; break;
            case SR_sss: sp[ op.o2 ] = sp[ op.o1 ] >> sp[ op.o0 ]; break;
            case SL_uuu: up[ op.o2 ] = up[ op.o1 ] << up[ op.o0 ]; break;

            case RR_uu:   up[ op.o1 ] = ( up[ op.o0 ] >> 1 )           | ( up[ op.o0 ] << 63 ); break;
            case RL_uu:   up[ op.o1 ] = ( up[ op.o0 ] << 1 )           | ( up[ op.o0 ] >> 63 ); break;
            case RR_cuu:  { u3_t s = op.o0 & 63; up[ op.o2 ] = ( up[ op.o1 ] >> s ) | ( up[ op.o1 ] << ( 64 - s ) ); } break;
            case RL_cuu:  { u3_t s = op.o0 & 63; up[ op.o2 ] = ( up[ op.o1 ] << s ) | ( up[ op.o1 ] >> ( 64 - s ) ); } break;
            case RR_uuu:  { u3_t s = up[ op.o0 ] & 63; up[ op.o2 ] = ( up[ op.o1 ] >> s ) | ( up[ op.o1 ] << ( 64 - s ) ); } break;
            case RL_uuu:  { u3_t s = up[ op.o0 ] & 63; up[ op.o2 ] = ( up[ op.o1 ] << s ) | ( up[ op.o1 ] >> ( 64 - s ) ); } break;

            /// unconditional jump
            case JP_C: pu.ip = code[ pu.ip ]; break;
            case JP_a: pu.ip =   up[ op.o0 ]; break;

            /// conditional jumps
            case JPZR_uA: pu.ip = ( up[ op.o0 ] == 0   ) ? code[ pu.ip ] : pu.ip + 1; break;
            case JPZR_fA: pu.ip = ( fp[ op.o0 ] == 0.0 ) ? code[ pu.ip ] : pu.ip + 1; break;
            case JPZR_ua: pu.ip = ( up[ op.o0 ] == 0   ) ? up[ op.o1 ] : pu.ip; break;
            case JPZR_fa: pu.ip = ( fp[ op.o0 ] == 0.0 ) ? up[ op.o1 ] : pu.ip; break;
            case JPNZ_uA: pu.ip = ( up[ op.o0 ] != 0   ) ? code[ pu.ip ] : pu.ip + 1; break;
            case JPNZ_fA: pu.ip = ( fp[ op.o0 ] != 0.0 ) ? code[ pu.ip ] : pu.ip + 1; break;
            case JPNZ_ua: pu.ip = ( up[ op.o0 ] != 0   ) ? up[ op.o1 ] : pu.ip; break;
            case JPNZ_fa: pu.ip = ( fp[ op.o0 ] != 0.0 ) ? up[ op.o1 ] : pu.ip; break;

            case JPEQ_uCA: pu.ip = ( up[ op.o0 ] ==         code[ pu.ip ] ) ? code[ pu.ip + 1 ] : pu.ip + 2; break;
            case JPNE_uCA: pu.ip = ( up[ op.o0 ] !=         code[ pu.ip ] ) ? code[ pu.ip + 1 ] : pu.ip + 2; break;
            case JPLG_uCA: pu.ip = ( up[ op.o0 ] >          code[ pu.ip ] ) ? code[ pu.ip + 1 ] : pu.ip + 2; break;
            case JPLG_sCA: pu.ip = ( sp[ op.o0 ] >  ( s3_t )code[ pu.ip ] ) ? code[ pu.ip + 1 ] : pu.ip + 2; break;
            case JPLG_fCA: pu.ip = ( fp[ op.o0 ] >  ( f3_t )code[ pu.ip ] ) ? code[ pu.ip + 1 ] : pu.ip + 2; break;
            case JPLE_uCA: pu.ip = ( up[ op.o0 ] >=         code[ pu.ip ] ) ? code[ pu.ip + 1 ] : pu.ip + 2; break;
            case JPLE_sCA: pu.ip = ( sp[ op.o0 ] >= ( s3_t )code[ pu.ip ] ) ? code[ pu.ip + 1 ] : pu.ip + 2; break;
            case JPLE_fCA: pu.ip = ( fp[ op.o0 ] >= ( f3_t )code[ pu.ip ] ) ? code[ pu.ip + 1 ] : pu.ip + 2; break;
            case JPSM_uCA: pu.ip = ( up[ op.o0 ] <          code[ pu.ip ] ) ? code[ pu.ip + 1 ] : pu.ip + 2; break;
            case JPSM_sCA: pu.ip = ( sp[ op.o0 ] <  ( s3_t )code[ pu.ip ] ) ? code[ pu.ip + 1 ] : pu.ip + 2; break;
            case JPSM_fCA: pu.ip = ( fp[ op.o0 ] <  ( f3_t )code[ pu.ip ] ) ? code[ pu.ip + 1 ] : pu.ip + 2; break;
            case JPSE_uCA: pu.ip = ( up[ op.o0 ] <=         code[ pu.ip ] ) ? code[ pu.ip + 1 ] : pu.ip + 2; break;
            case JPSE_sCA: pu.ip = ( sp[ op.o0 ] <= ( s3_t )code[ pu.ip ] ) ? code[ pu.ip + 1 ] : pu.ip + 2; break;
            case JPSE_fCA: pu.ip = ( fp[ op.o0 ] <= ( f3_t )code[ pu.ip ] ) ? code[ pu.ip + 1 ] : pu.ip + 2; break;
            case JPEQ_uuA: pu.ip = ( up[ op.o0 ] == up[ op.o1 ] ) ? code[ pu.ip ] : pu.ip + 1; break;
            case JPNE_uuA: pu.ip = ( up[ op.o0 ] != up[ op.o1 ] ) ? code[ pu.ip ] : pu.ip + 1; break;
            case JPLG_uuA: pu.ip = ( up[ op.o0 ] >  up[ op.o1 ] ) ? code[ pu.ip ] : pu.ip + 1; break;
            case JPLG_ssA: pu.ip = ( sp[ op.o0 ] >  sp[ op.o1 ] ) ? code[ pu.ip ] : pu.ip + 1; break;
            case JPLG_ffA: pu.ip = ( fp[ op.o0 ] >  fp[ op.o1 ] ) ? code[ pu.ip ] : pu.ip + 1; break;
            case JPLE_uuA: pu.ip = ( up[ op.o0 ] >= up[ op.o1 ] ) ? code[ pu.ip ] : pu.ip + 1; break;
            case JPLE_ssA: pu.ip = ( sp[ op.o0 ] >= sp[ op.o1 ] ) ? code[ pu.ip ] : pu.ip + 1; break;
            case JPLE_ffA: pu.ip = ( fp[ op.o0 ] >= fp[ op.o1 ] ) ? code[ pu.ip ] : pu.ip + 1; break;
            case JPSM_uuA: pu.ip = ( up[ op.o0 ] <  up[ op.o1 ] ) ? code[ pu.ip ] : pu.ip + 1; break;
            case JPSM_ssA: pu.ip = ( sp[ op.o0 ] <  sp[ op.o1 ] ) ? code[ pu.ip ] : pu.ip + 1; break;
            case JPSM_ffA: pu.ip = ( fp[ op.o0 ] <  fp[ op.o1 ] ) ? code[ pu.ip ] : pu.ip + 1; break;
            case JPSE_uuA: pu.ip = ( up[ op.o0 ] <= up[ op.o1 ] ) ? code[ pu.ip ] : pu.ip + 1; break;
            case JPSE_ssA: pu.ip = ( sp[ op.o0 ] <= sp[ op.o1 ] ) ? code[ pu.ip ] : pu.ip + 1; break;
            case JPSE_ffA: pu.ip = ( fp[ op.o0 ] <= fp[ op.o1 ] ) ? code[ pu.ip ] : pu.ip + 1; break;
            case JPEQ_uua: pu.ip = ( up[ op.o0 ] == up[ op.o1 ] ) ? up[ op.o2 ] : pu.ip; break;
            case JPNE_uua: pu.ip = ( up[ op.o0 ] != up[ op.o1 ] ) ? up[ op.o2 ] : pu.ip; break;
            case JPLG_uua: pu.ip = ( up[ op.o0 ] >  up[ op.o1 ] ) ? up[ op.o2 ] : pu.ip; break;
            case JPLG_ssa: pu.ip = ( sp[ op.o0 ] >  sp[ op.o1 ] ) ? up[ op.o2 ] : pu.ip; break;
            case JPLG_ffa: pu.ip = ( fp[ op.o0 ] >  fp[ op.o1 ] ) ? up[ op.o2 ] : pu.ip; break;
            case JPLE_uua: pu.ip = ( up[ op.o0 ] >= up[ op.o1 ] ) ? up[ op.o2 ] : pu.ip; break;
            case JPLE_ssa: pu.ip = ( sp[ op.o0 ] >= sp[ op.o1 ] ) ? up[ op.o2 ] : pu.ip; break;
            case JPLE_ffa: pu.ip = ( fp[ op.o0 ] >= fp[ op.o1 ] ) ? up[ op.o2 ] : pu.ip; break;
            case JPSM_uua: pu.ip = ( up[ op.o0 ] <  up[ op.o1 ] ) ? up[ op.o2 ] : pu.ip; break;
            case JPSM_ssa: pu.ip = ( sp[ op.o0 ] <  sp[ op.o1 ] ) ? up[ op.o2 ] : pu.ip; break;
            case JPSM_ffa: pu.ip = ( fp[ op.o0 ] <  fp[ op.o1 ] ) ? up[ op.o2 ] : pu.ip; break;
            case JPSE_uua: pu.ip = ( up[ op.o0 ] <= up[ op.o1 ] ) ? up[ op.o2 ] : pu.ip; break;
            case JPSE_ssa: pu.ip = ( sp[ op.o0 ] <= sp[ op.o1 ] ) ? up[ op.o2 ] : pu.ip; break;
            case JPSE_ffa: pu.ip = ( fp[ op.o0 ] <= fp[ op.o1 ] ) ? up[ op.o2 ] : pu.ip; break;

            case CSR_A: data[ pu.sp++ ] = pu.ip + 1; pu.ip = code[ pu.ip ]; break;
            case CSR_a: data[ pu.sp++ ] = pu.ip; pu.ip = up[ op.o0 ]; break;
            case RSR:   pu.ip = data[ --pu.sp ]; break;

            case PUS_x: data[ pu.sp++ ] = up[ op.o0 ];  break;
            case PUS_a: data[ pu.sp++ ] = data[ up[ op.o0 ] ];  break;
            case PUS_I: data[ pu.sp++ ] = pu.ip;  break;
            case PUS_S: data[ pu.sp ] = pu.sp;  pu.sp++; break;
            case POP_x: up[ op.o0 ]         = data[ --pu.sp ];  break;
            case POP_a: data[ up[ op.o0 ] ] = data[ --pu.sp ];  break;
            case POP_I: pu.ip               = data[ --pu.sp ];  break;

            default: break;
        }
    }

    if( pu.sp >= data_size ) ERR_fa( "stack overflow" );
}

/**********************************************************************************************************************/

static st_s* machine_selftest( void )
{
    bcore_life_s* l = bcore_life_s_create();

    bclos_mcode_s*  code = bcore_life_s_push_aware( l, bclos_mcode_s_create() );
    bcore_arr_u3_s* data = bcore_life_s_push_aware( l, bcore_arr_u3_s_create() );

    // selftest calculating pi(max) (number of primes <= max) and the largest prime <= max.
    u3_t max = 1000000;
    bcore_arr_u3_s_fill( data, 1000000, 0 );

    enum
    {
        VAR = 0, // prime test variable
        MAX,     // limit for prime numbers
        INDEX,   // index for prime array on stack
        PRIME,   // actual prime from stack
        TEST     // test container
    };

    bclos_mcode_s_push_op2( code, CP_cx, 2, PRIME ); // first 2 primes
    bclos_mcode_s_push_op1( code, PUS_x, PRIME );
    bclos_mcode_s_push_op2( code, CP_cx, 3, PRIME );
    bclos_mcode_s_push_op1( code, PUS_x, PRIME );

    bclos_mcode_s_push_op2( code, CP_cx, 4, VAR ); // set VAR
    bclos_mcode_s_push_op2( code, CP_Cx, 0, MAX ); // set MAX
    bclos_mcode_s_push_u3(  code, max );

    u3_t outer_loop = code->arr.size; // outer loop
        bclos_mcode_s_push_op1( code, INC_u, VAR ); // var++
        bclos_mcode_s_push_op2( code, JPEQ_uuA, VAR, MAX ); // exit loop if var == max
        u3_t exit_loop_i1 = code->arr.size; // inner loop
        bclos_mcode_s_push_u3(  code, 0 );

        bclos_mcode_s_push_op2( code, CP_cx, 0, INDEX );     // index = 0
        bclos_mcode_s_push_op2( code, CP_ax, INDEX, PRIME ); // get prime

        u3_t inner_loop = code->arr.size; // inner loop
            bclos_mcode_s_push_op3( code, MD_uuu, VAR, PRIME, TEST );   // test = var % prime
            bclos_mcode_s_push_op1( code, JPZR_uA, TEST );              // back to outer loop if test == 0 (var is not prime)
            bclos_mcode_s_push_u3(  code, outer_loop );

            bclos_mcode_s_push_op1( code, INC_u, INDEX ); // index++
            bclos_mcode_s_push_op2( code, CP_ax, INDEX, PRIME );        // get prime
            bclos_mcode_s_push_op3( code, ML_uuu, PRIME, PRIME, TEST ); // test = prime ^ 2
            bclos_mcode_s_push_op2( code, JPSE_uuA, TEST, VAR );        // back to inner loop if test <= var (continue testing)
            bclos_mcode_s_push_u3(  code, inner_loop );

            // var is prime
            bclos_mcode_s_push_op1( code, PUS_x, VAR ); // push var
            bclos_mcode_s_push_op0( code, JP_C );       // back to outer loop if test < var (continue testing)
            bclos_mcode_s_push_u3(  code, outer_loop );

    u3_t exit_loop = code->arr.size; // outer loop
    code->arr.data[ exit_loop_i1 ] = exit_loop;

    bclos_mcode_s_push_op2( code, CP_Sx, 0, TEST ); // TEST = number of primes
    bclos_mcode_s_push_op1( code, POP_x, PRIME );   // PRIME = last prime
    bclos_mcode_s_push_op2( code, CP_cx, 0, VAR );  // reset stack pointer
    bclos_mcode_s_push_op1( code, CP_xS, VAR );

    bclos_mcode_s_push_op1( code, PUS_x, PRIME );  // last prime
    bclos_mcode_s_push_op1( code, PUS_x, TEST );   // number of primes

    bclos_machine_run( code->arr.data, code->arr.size, data->data, data->size );

    u3_t largest = data->data[ 0 ];
    u3_t pi = data->data[ 1 ];

    ASSERT( pi == 78498 );
    ASSERT( largest == 999983 );

    bcore_life_s_discard( l );
    return NULL;
}

/**********************************************************************************************************************/

vd_t bclos_machine_signal( tp_t target, tp_t signal, vd_t object )
{
    if( target != typeof( "all" ) && target != typeof( "bclos_machine" ) ) return NULL;

    if( signal == typeof( "init1" ) )
    {
        bcore_flect_define_creator( typeof( "bclos_minst_s" ), bclos_minst_s_create_self );
        bcore_flect_define_creator( typeof( "bclos_mcode_s" ), bclos_mcode_s_create_self );
        bcore_flect_define_creator( typeof( "bclos_mcpu_s"  ), bclos_mcpu_s_create_self  );
    }
    else if( signal == typeof( "selftest" ) )
    {
        return machine_selftest();
    }

    return NULL;
}

