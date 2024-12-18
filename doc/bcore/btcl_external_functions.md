# External Functions
BTCL can access stamp member functions by implementing the features 

``` C
feature sz_t btcl_function_arity( @* o, tp_t name ) = -1; // return -1 when function 'name' is not defined
feature er_t btcl_function(       @* o, tp_t name, bcore_arr_sr_s* args, m sr_s* result ); // must handle all names as indicated by btcl_function_arity
```
These define a set of functions. Each can be used in btcl like a member function of the stamp that implements the features.

### Example: Definition (XOILA)

``` C
name add_a;

stamp my_stamp
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
             default: break; // never reached
         }
         = 0;
    }
}
```

### Example: Usage (BTCL)

``` C
obj = <my_stamp/>( .additive = 10 );
? obj.add_a( 1 ) // outputs '11'
```


------
<sub>&copy; 2024 Johannes B. Steffens</sub>