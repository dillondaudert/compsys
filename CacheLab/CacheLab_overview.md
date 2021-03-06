##Cache Lab project plan, specification, design draft

###References
- [YouTube PlayList on Caches](https://www.youtube.com/watch?v=chnhnxWIjgw&list=PLbtzT1TYeoMgJ4NcWFuXpnF24fsiaOdGq&index=1)
- [The Open Group Group Specifications, getopt()](http://pubs.opengroup.org/onlinepubs/9699919799/)

###Overall Goal
Write a cache simulator that keeps track of hits, misses, and evictions

####Files
#####csim.c 
contains all the code, described in the simulator section

#####traces/* 
a collection of reference trace files used to evaluate correctness of the simulator

the format is:
```[space]operation address,size
```

`operation` denotes the type of memory access:
- "I" instruction load
- "L" data load
- "S" data store
- "M" data modify (data load followed by data store)

There is no space before the "I" operation, otherwise there is a space.

`address` specifies a 64-bit hexadecimal memory address.

`size` specifies the number of bytes accessed by the operation.

####Cache Simulator Specs
The simulator:
1) takes a memory trace from `traces/*` as input
2) simulates the hit/miss behavior of a cache memory on this trace
3) outputs the total number of hits, misses, and evictions

#####Invocation
Usage: `./csim [-hv] -s <s> -E <E> -b <b> -t <tracefile>`
- `-h`: Optional help flag that prints usage info
- `-v`: Optional verbose flag that displays trace info
- `-s <s>`: Number of set index bits (_S_ = 2^_s_ is the number of sets)
- `-E <E>`: Associativity (number of lines per set)
- `-b <b>`: Number of block bits (_B_ = 2^_b_ is the block size)
- `-t <tracefile>`: Name of the `trace/*` file to replay

#####Output
Without `-v`
`hits:4 misses:5 evictions:3`

With `-v`
```L 10,1 miss

M 20,1 miss hit

L 22,1 hit

S 18,1 hit

...

hits:4 misses:5 evictions:3
```


###Cache Simulator Design Outline
- command line argument parser
- trace file IO
- cache simulator program
  * initialization (per command line arguments)
  * operation functions
  * teardown
- output function

####Command Line Argument Parser
Use getopt() from unistd.h to parse the proper flags
```int getopt(int argc, char * const argv[], const char *optstring);
extern char *optarg;
extern int optind, opterr, optopt;
```

_optstring_ is a string containing the valid characters, with characters followed by a colon indicating that an argument is required.
`char *optstring = ":hvs:E:b:t:"`
The option switch statement will looking something along these lines:
```C
int c;
while (c = getopt(argc, argv, optstring) != -1){
  switch(c){
    case '?':
      //invalid option, print usage and exit
      break;
    case ':':
      //option missing argument, print usage and exit
      break;
    case ...
  }
}
  
```

####Trace File IO

####Cache Simulator Program
#####Initialization
Construct a cache of arbitrary dimension for 64-bit addresses.

The size of the cache in bytes is defined as:
  `C = S (sets in the cache) x E (lines per set) x B (block size in bytes)`.
  
In addition, each line requires a valid bit denoting whether or not valid memory has been loaded into that block.

So the lines of each set of the cache will end up looking like:

```[valid | tag | block bytes] [valid | tag | block bytes] ...
```

#####Cache Operations
Cache Read (given an address 64 bits: [tag | set index | block offset])
- locate the set 
- check the tags of all *valid* lines in set (line: [valid | tag | block bytes])
- if tag matches, then *hit*
- if no tag matched, then *miss*
  - replace the *least recently used* line (update valid bit and tag in this case)
  
Cache Write
- search cache for block
- if found in cache:
  - write-through: modify block and write to memory
  - write-back: modify block, flag dirty bit, and write to memory once evicted (_typical_)
- if not found in cache:
  - write-allocate: read block into the cache, update copy in cache, flag dirty bit (_typical_)
  - write-no-allocate: send the write on through to memory, do not load into cache

####Output Function
