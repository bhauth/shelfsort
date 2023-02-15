


## Shelfsort: a new sorting algorithm

#### What basic properties does the Shelfsort algorithm have?
- O(N * log(N)) time
- O(sqrt(N)) memory (with constant pointer size)
- adaptive (fast for mostly pre-sorted inputs)
- stable (order of equal elements is unchanged)

#### What's novel about the Shelfsort algorithm?
The core idea is:
1. Divide the data into blocks of sqrt(N) size, and sort each block.
2. Use sqrt(N) indices to track which block is where.
3. Clear some blocks by merging data into scratch memory.
4. Merge blocks of data from where they are into whichever blocks are empty.

Here's an example of merging blocks:

1. The data is divided into blocks.  
``aaaabbbb``

2. Merging of {a, b} into {C} starts. The blocks of {a, b} are merged into a buffer, starting with the first ones. When blocks are used completely, they're cleared in the main array. Here, {b} is first in the sort, so blocks of {b} are cleared first.  
``aaaa__bb``

3. The merged run data {C} is written into the space left by cleared {b} blocks. The location indices are stored in a locations array. Here, locations[i] is the index where the ith output data block can be found.  
``aaaaCC_b``  
``45______``

4. Here, {a, b} are fully merged into {C} and the locations are stored. If another merge is done, blocks are used from their current positions.  
``CCCCCCCC``  
``45670123``

5. Once all data has been merged, the blocks are sorted according to their location indices.

#### How fast is Shelfsort?
For large arrays, compared to std::stable_sort with unlimited memory, this implementation takes: 
- ~1.1x the time for random inputs
- ~0.1x the time for sorted inputs

[Here are some times](times.md) I got from running the included code.

#### Why name it "Shelfsort"?
The block locations are fixed while elements move between them, like books being moved between shelves. Also, "block sort" was taken.

#### Why a new sorting algorithm?
Most sorting algorithms are either unstable, slow, or require O(N) memory. There are merge sorts using binary search to partition blocks to reduce memory consumption, such as [blitsort](https://github.com/scandum/blitsort/), but this is a different approach that doesn't use binary search.

#### No, I meant, why did you work on this?
- It's an intellectual challenge that's "pure" in a way that I like.
- It's a way to test yourself against some of the best minds of history.
- If you can develop something situationally useful, you can leave some small mark on the world.
- I'm currently unemployed, and the weather's been bad, so I had some free time.
- It will probably get HR people to not throw out my resume. <- this is sarcasm

#### How optimized is this implementation?
It's written in low-level C++ and does some branchless operations, but apart from that, it's not very optimized. Things I *haven't* done include:
- trying initial sorts for small segments that handle >4 elements
- looking at the assembly code
- manual vectorization

Micro-optimization isn't my specialty or my interest, but feel free to try doing some.

#### What limitations does this implementation have?
- To simplify implementation and discourage people from using this code in production, it currently only works on power-of-2 array sizes.
- It hasn't been tested thoroughly, and could have bugs on some inputs.

#### Have you made other open-source stuff?
I also programmed:
- a [fork of a roguelike game](https://github.com/b-crawl/bcrawl/)
- a [proof of concept](https://github.com/bhauth/multilevel-ternary-hash-table) for a new hashtable algorithm

#### How can I contact you?
b αt bhauth dot com




