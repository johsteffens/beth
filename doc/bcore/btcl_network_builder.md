# Network Builder
The network builder allows constructing a network or graph composed of nodes en edges. 

## Usage
The graph is intended as meta structure use by an external builder to general the final object, process or program. How exactly the graph is interpreted is up to that builder.

Each node carries a source point to pass on source information to the external builder.

A typical use case is the construction of signal processing components in a network of data flow. E.g. Video processing; audio processing such as synthesizer and sequencer design. The syntax examples below refer to the synthesize application 'Symbre'.

## Tree
The principal structure of the graph is a tree consisting of nodes and branches. For descriptive purposes we imagine the tree to branch out from left to right. Hence the leftmost node is the root of the tree.

A generic tree-node carries a name and maintains a list of named branches. Names can also be empty, in which case the branch is considered anonymous. 

### Wires
Additional connections between nodes can be made though wires. Wires can represent edges outside a valid tree structure, thus extending the tree to a general purpose graph.

### Leafs
A leaf is a node with no further branches. Any beth-object can be a leaf node. The external builder may impose additional restrictions.

## Nodes

|Syntax|Description|btcl object|
|:---|:---|:---|
|```@<name>```|Generic node|```x_btcl_net_node```|
|```@:<name>```|Rack|```x_btcl_net_node```|
|```@~<name>```|Wire to rack of ```<name>```|```x_btcl_net_node```|

### Using operators on nodes

BCTL exports operators on nodes. 

The external builder should treat operators as nodes with one or more anonymous branches according to the operator's arity.

**Example:**

```C
a = 1;
b = 2;
c = a + b; // c is immediately reduced to 3

a = @sine;
b = @triangle;
c = a + b; // c is the btcl operator'plus' with anonymous braches 'a' and 'b';

```

## Branches

To push a named branch use the modifier functional syntax ```.<name> = <expression>```.

Branch names are unique per node. Pushing an existing name replaces the named branch.

To push an anonymous branch use the modifier syntax without a name specification. (s. example below)

## Rack

A rack is a specialized node to which wires can connect.

A rack can be anonymous.

A rack has an arbitrary amount of voice branches and optional named branches which can serve as wire-sockets.

Only wires in one of the rack's branches can connect to the rack.

Multiple racks of the same name are allowed in the tree.

## Wire

A wire is a leaf node which allows arbitrary connection to another points in the network tree. Thus, a wire is not considered an edge of the tree. But it represents an edge of the overall graph.

A wire identifies the connection by specifying the rack name and branch name of the rack; for the main branch, the name stays empty.

A wire only connects to a matching rack on the left with the shortest (tree-)path. 

A wire with no matching rack is considered *open*. The external builder decides how to handle open wires.

## Operators

The network builder uses all [btcl-operators](btcl.md#operators) in their respective order of priority . 

The following operators represent a special behavior:

### Group C - Binary

| Symbol   | Description                                | Order |
| :------- | :----------------------------------------- | ----- |
| ```<<``` | x_btcl_net_node: Pushes an anonymous node. | RL    |


## Syntax

### Generic nodes

Branches are assigned to a node with the same syntax as objects are assigned to object-elements.

Anonymous branches are assigned like arguments to a function.

**Example:**

```C
lfo = @sine( .frq = 1 );
mynode1 = @sine( .frq = 1000, .length = 10 );
mynode2 = @rect( .frq = 1000, .length = 10 );
mynode3 = mynode1 + mynode2;

// setup a filter node with main (anonymous) branch 'mynode3'
@filter_biquad( .type = "lp" .frq = 2000 + lfo * 1000, mynode3 );

// also possible
@filter_biquad( .type = "lp" .frq = 2000 + lfo * 1000 ) << mynode3;
```

### Wiring
Rack and wires are named by the rack name. In case of wiring to a named branch, a wire uses a delimiter ```.<name>``` to specify the branch name.

**Example: (wiring to main rack branch)**

```C
simple_loop = @:rk( @sine( .frq = 1000 ) - @~rk );
```

**Example: (wiring to named rack branch)**

```C
lfo = @sine( .frq = 2 );
vibrato = @:rk( .lfo = lfo, @sine( .frq = 1000 + 100 * @~rk.lfo ) );
```
### Functions
When a function is passed as parameter, it is converted into a functor.

<sub>&copy; 2024 Johannes B. Steffens</sub>