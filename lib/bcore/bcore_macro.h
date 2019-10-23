/** Author and Copyright 2019 Johannes Bernhard Steffens
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

/**********************************************************************************************************************/

/// Collection of useful macros

/**********************************************************************************************************************/

#ifndef BCORE_MACRO_H
#define BCORE_MACRO_H

// ---------------------------------------------------------------------------------------------------------------------
// Arbitrary concatenation

/// Returns the number of variadic arguments passed (max 8 args)
#define BCORE_VA_ARGC( ... ) BCORE_VA_ARGC_( __VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1 )
#define BCORE_VA_ARGC_( a1, a2, a3, a4, a5, a6, a7, a8, argc, ... ) argc

/// Returns the first argument is a variadic list (discards rest)
#define BCORE_VA_HEAD( ... ) BCORE_VA_HEAD_( __VA_ARGS__ )
#define BCORE_VA_HEAD_( a1, ... ) a1

/// Returns the variadic list except the first argument
#define BCORE_VA_TAIL( ... ) BCORE_VA_TAIL_( __VA_ARGS__ )
#define BCORE_VA_TAIL_( a1, ... ) __VA_ARGS__

/// BCORE_CATF_<n> evaluates n expressions and concatenates results with pattern 'f' between.
#define BCORE_CATF_1( f, a1 ) a1
#define BCORE_CATF_2( f, ... ) BCORE_CAT_3(  BCORE_VA_HEAD( __VA_ARGS__ ), f, BCORE_VA_TAIL( __VA_ARGS__ ) )
#define BCORE_CATF_3( f, ... ) BCORE_CATF_2( f, BCORE_VA_HEAD( __VA_ARGS__ ), BCORE_CATF_2( f, BCORE_VA_TAIL( __VA_ARGS__ ) ) )
#define BCORE_CATF_4( f, ... ) BCORE_CATF_2( f, BCORE_VA_HEAD( __VA_ARGS__ ), BCORE_CATF_3( f, BCORE_VA_TAIL( __VA_ARGS__ ) ) )
#define BCORE_CATF_5( f, ... ) BCORE_CATF_2( f, BCORE_VA_HEAD( __VA_ARGS__ ), BCORE_CATF_4( f, BCORE_VA_TAIL( __VA_ARGS__ ) ) )
#define BCORE_CATF_6( f, ... ) BCORE_CATF_2( f, BCORE_VA_HEAD( __VA_ARGS__ ), BCORE_CATF_5( f, BCORE_VA_TAIL( __VA_ARGS__ ) ) )
#define BCORE_CATF_7( f, ... ) BCORE_CATF_2( f, BCORE_VA_HEAD( __VA_ARGS__ ), BCORE_CATF_6( f, BCORE_VA_TAIL( __VA_ARGS__ ) ) )
#define BCORE_CATF_8( f, ... ) BCORE_CATF_2( f, BCORE_VA_HEAD( __VA_ARGS__ ), BCORE_CATF_7( f, BCORE_VA_TAIL( __VA_ARGS__ ) ) )

#define BCORE_CAT_2( a1, a2 ) BCORE_CAT_2_( a1, a2 )
#define BCORE_CAT_2_( a1, a2 ) a1##a2

#define BCORE_CAT_3( a1, a2, a3 ) BCORE_CAT_3_( a1, a2, a3 )
#define BCORE_CAT_3_( a1, a2, a3 ) a1##a2##a3

/// Evaluates and concatenates a flexible amount of (max 8) arguments with pattern 'f' between
#define BCATF( f, ... ) BCORE_CAT_2( BCORE_CATF_, BCORE_VA_ARGC( __VA_ARGS__))( f, __VA_ARGS__ )

/// Evaluates and concatenates a flexible amount of (max 8) arguments
#define BCAT( ... ) BCATF( , __VA_ARGS__ )

/// Evaluates and concatenates an flexible amount of (max 8) arguments with '_' between
#define BCATU( ... ) BCATF( _, __VA_ARGS__ )

// ---------------------------------------------------------------------------------------------------------------------
// string

/// Evaluates and encloses argument in quotes
#define BSTR( a ) BSTR_( a )
#define BSTR_( a ) #a

#endif // BCORE_MACRO_H
