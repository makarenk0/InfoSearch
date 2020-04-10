# InfoSearch
Search engine C++
Provides different methods of files indexing

## Can generate:<br>
* inverted index (.txt and in RAM)<br>
* biword index (.txt and in RAM)<br>
* positional index (.txt and in RAM)<br>
* permuterm index (only in RAM)<br>
* threegram index (only in RAM)<br>

## Can execute:<br>
* boolean search (Supported operators: "OR" "AND" "NOT", nesting)<br>
 Example: "NOT zone OR NOT zones AND ( zoo OR zoom )" <br><br>
* phrase search (Any amount of words)<br>
 Example: "curious to recall some"<br><br>
* proximity search (After slash write a range in which program has to find word)<br>
 Example: "zone /3 zones" - finds "zone" where<br> 
 in range of 3 words there is "zones"<br><br>
* post wildcard queries (Have to do pre wildcard query to combine in free wildcard(extra))<br>
 Example: ```"compute*"``` finds words with missed char<br><br>
* free wildcard queries on permuterm index(based on own BTree realization)<br>
 Example: ```"com*uter"```<br><br>
* free wildcard queries on threegram index(also BTree)<br>
 Example: ```"com*uter"```<br><br>
## Big data algorithms
* SPIMI algorithm<br><br>
## Compression:<br>
* dictionary in RAM compressing
* postings lists: byte, 4-block encoding (both RAM and Hard drive) + word frequency
* -
## Interface
* WPF MVVM, .NET Core platform (C# and C++)
Tests:<br>
Gutenberg collection<br>
## Machines
### i3-8130u(8 gb, M2 SSD)<br> 
### i5-9400F(8 gb, SSD)<br>
**All results in folder "tests"**
