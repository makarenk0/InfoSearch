# InfoSearch
Search engine C++
Provides different methods of files indexing
--------------------------------------------
Can generate:
-inverted index (.txt and in RAM)
-biword index (.txt and in RAM)
-positional index (.txt and in RAM)
-permuterm index (only in RAM)
-threegram index (only in RAM)
--------------------------------------------
Can execute:
-boolean search (Supported operators: "OR" "AND" "NOT", nesting) Example: "NOT zone OR NOT zones AND ( zoo OR zoom )" 
-phrase search (Any amount of words) Example: "curious to recall some"
-proximity search (After slash write a range in which program has to find word) Example: "zone /3 zones" - finds "zone" where 
                                                                                             in range of 3 words there is "zones" 
-post wildcard queries (Have to do pre wildcard query to combine in free wildcard(extra)) Example: "compute*" finds words with missed char
-free wildcard queries on permuterm index(based on own BTree realization) Example: "com*uter"
-free wildcard queries on threegram index(also BTree) Example: "com*uter"
--------------------------------------------
Compression:
-SPIMI algorithm

--------------------------------------------
Tests:
Gutenberg collection
Machines: i3-8130u(8 gb, M2 SSD), i5-9400F(8 gb, SSD) results in folder "tests"
