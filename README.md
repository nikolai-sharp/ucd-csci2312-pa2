# ucd-csci2312-pa2

 Compiler:
 I used CLion 1.1.1 in mac. I believe this uses GCC.
 
 One of my first goals in either class was to get a working overloaded ostream operattor (<<) working.
 This allowed me to test via the main function as I went.
 
 Point class: 
 I feel satisfied with the operation of this class. It works quite well.
 
 
 Cluster class:
 The brunt of the work is placed on the add function, which attempts to keep the code organized by both value and
 address. The idea behind this is to allow easy implementation of == operator overload. If points equal in value
 are organized by their address within a class, I can just loop through all the values of two classes and test if
 the addresses of the points they point to are equal.
 
 While I leave you with code that compiles and has all functions available and working, I am not satisfied
 that they work perfectly accurately.
 For example, as can be seen in the final few tests, cl1 + cl2 did not equal cl2 + cl1.
 I believe most of my problems come from an overly complicated logic structure when attempting the forementioned
 organization.
 
 I will continue work on this in coming free time.
 
 
 
 Apologies for the brevity of this readme. I almost guessed the time I would need correctly. However, you 
 will find my comments to be fairly extensive (though inaccurate in a couple cases). 