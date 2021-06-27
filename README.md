# Arrays-Library

This is an Implementation of Arrays as an abstract data type. Better than std::vector in certain aspects.

A lot of functionalities have been provided in this library, can be used robustly in big projects. 

This Array Class is designed to behave like a vector but in a permitted range of 0 - MAX_ARRAY_CAPACITY.

Compatibility:

    - With the algorithm library
    - for each loops
    - any other Cpp library that accesses a contigious memory block using begin() and end() method.

Supports:

		- Negative Index Accessibility
		- Negative Index Insertability
		- Sorted Insertability
		- Input via operator>>
		- Appending
		- Concatenation
		- Replacment
		- Poping
		- Sorting
		- Reversal
		- Erasing
		- Max and Min
		- Duplicates Removal
		- Count and Conditional Count
		- Conditional Removal and Complete removal (all occurrences) of an Element
		- Linear and Binary Search
		- Lower and Upper Bound Search
		- Rotation in Clockwise and Anti Clockwise Direction
		- Negative Positive Rearrangement

Operators Supported:

		- operator<<	: shows elements to the console
		- operator>> 	: requires that the size is set during initialization
		- operator=		: Assignment
		- operator+		: Momentary concatenation
		- operator+=	: by reference concatenation of a single element or initialiser_list
		- operator[]	: subscripting
		- all of the relational operators < , > , <= , >= , == , !=
				> Uses std::lexicographical_compare
		- operator() 	: it takes a function as a parameter and maps the function to all the array elements

Iterators Supported:

		- iterator			: random access iterator
		- const_iterator 		: const random access iterator
		- reverse_iterator		: random access reverse iterator
		- const_reverse_iterator	: const random access reverse iterator

Other memory Manipulation functions Supported:

		- resize()
		- clear()
		- shrink_to_fit()
    
Note: There is no certainity of heap invalidation/corruption in projects that use multithreading. while work is still in progress. once this is solved, will update here.
