# Standard External Functions

BTCL can execute stamp functionality with stamps implementing the features below. These define a set of functions. Each can be used in BTCL source code like a member function call.

## Features

``` C
// (required) return -1 when function 'name' is not defined
feature sz_t btcl_function_arity( @* o, tp_t name ) = -1;

// Overload one or both of the features below depending on whether 'name' is mutable for o or not.
// If result is not explicitly set, it defaults to 0 (<f3_t> 0 </>)
feature er_t btcl_function( @* o, tp_t name, x_source_point_s* sp, m :frame_s* lexical_frame, bcore_arr_sr_s* args, m sr_s* result );

feature er_t m_btcl_function( m@* o, tp_t name, x_source_point_s* sp, m :frame_s* lexical_frame, bcore_arr_sr_s* args, m sr_s* result );

// (optional) indicates whether the function given by 'name' is mutable
// Only needed when both btcl_function and m_btcl_function are overloaded
feature bl_t btcl_function_mutable( @* o, tp_t name );
```

### Example: Definition (XOILA)


``` C
/// Minimal implementation: Simply calls a function of name run(), returns default value 0 (<f3_t>)
name run;
stamp my_minimal_s
{
    func er_t run( m@* o ) { bcore_msg_fa( "BTCL (#name): 'run' called\n", o._ ); = 0; }
    func x_btcl.btcl_function_arity = ( name == run~ ) ? 0 : -1;
    func x_btcl.m_btcl_function     = ( name == run~ ) ? o.run() : 0;
}
```

``` C
name add_a;

/// Advanced implementation for multiple functions and custom return value
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
? obj.add_a( 1 ); // outputs '11'
    
? <my_minimal_s/>.run(); // executes 'my_minimal_s.run'; outputs '0'

```


# Parsing-Function 

The parsing function receives full parsing control of a source code section. it takes responsibility for parsing the designated code section and also recognizing the proper end of that section and thus returning control to the btcl parser at the correct place. In that way, the parsing function can impose an alternative syntax onto the btcl source code.

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