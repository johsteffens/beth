# Standard External Functions

BTCL can execute stamp functionality with stamps implementing the features below.

## Features

``` C
// (required) return -1 when function 'name' is not defined
feature sz_t btcl_function_arity( @* o, tp_t name ) = -1;

// (optional) indicates if calling the btcl function changes the underlying instance; 
// If true, btcl creates a copy before calling.
feature bl_t btcl_function_mutable( @* o, tp_t name ) = false;

// Depending on mutability one or both of the following functions must be implemented.
// All names declared by arity must be handled in one of these functions.

// Overload for all const functions
feature er_t btcl_function( @* o, tp_t name, x_source_point_s* sp, m :frame_s* lexical_frame, bcore_arr_sr_s* args, m sr_s* result );

// Overload for all mutable functions
feature er_t m_btcl_function( m@* o, tp_t name, x_source_point_s* sp, m :frame_s* lexical_frame, bcore_arr_sr_s* args, m sr_s* result );
```
These define a set of functions. Each can be used in BTCL source code like a member function call.

### Example: Definition (XOILA)

``` C
name add_a;

stamp my_stamp_s
{
    f3_t additive = 0;
    
    /* Must return the arity of the function specified by name.
     * Must return -1 for any invalid name.
     */
    func x_btcl.btcl_function_arity
    {
         switch( name )
         {
             case add_a~: =  1; // add_a accepts one argument
             default:     = -1;
         }
         = -1;
    }
    
    /// Implements all functions which names are listed in the arity function above.
    func x_btcl.btcl_function
    {
         switch( name )
         {
             case add_a~: result.from_f3( args.[0].to_f3() + o.addditive ); break;
             default: sp.parse_error_fa( "Invalid function name '#<sc_t>'.", bnameof( name ) );
         }
         = 0;
    }
}
```

### Example: Usage (BTCL)

``` C
obj = <my_stamp_s/>( .additive = 10 );
? obj.add_a( 1 ) // outputs '11'
```


# Parsing External Functions 

Parsing function receive full parsing control of a source code section. The parsing function takes responsibility for parsing the designated code section and also recognizing the proper end of that section and thus returning control to the btcl parser at the correct place. In that way the parsing function can impose an alternative syntax onto the btcl source code.

## Features

``` C
// Overload one of these features depending on whether o is mutable or not.
feature 'at' er_t   btcl_external_parse( c@* o, m x_source* source, m :frame_s* lexical_frame, m sr_s* result );
feature 'at' er_t m_btcl_external_parse( m@* o, m x_source* source, m :frame_s* lexical_frame, m sr_s* result );
```

 *  Overloading these features lets BTCL pass parsing control to an external parse function.
 *  The external code section ends before an unmatched closing bracket ')' or at ```eos```.
 *  The external parse function must either consume all characters of its section or generate an error condition. It is permitted to leave BTCL whitespaces unconsumed.
 *  In BTCL source code, the parse function is called as member function to the object using keyword `parse` or `parse_file`

### Example: Definition (XOILA)

``` C
// This example parser simply copies all character of the parse section into a string
stamp my_parser_s
{
    func x_btcl.btcl_external_parse
    {
        m$* string = st_s!^;
        while( !source.eos() && ( source.inspect_char() != ')' ) )
        {
            string.push_char( source.get_char() );
        }
        result.asm( string.fork() );
        = 0;
    }
}
```

### Example: Usage (BTCL)

``` C
obj = <my_parser_s/>;
ASSERT( obj.parse( abcd ) == "ABCD" );
ASSERT( obj.parse_file( "MyFile.txt" ) == "ABCD" ); // assuming that MyFile.txt contains the text 'ABCD'

```



<sub>&copy; 2025 Johannes B. Steffens</sub>