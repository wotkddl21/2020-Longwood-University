I made 2 big functions and 3 small functions.

At first I made check_affine function.
In this function, I used the fact that
x_2 = a_2^-1(a_1*x_1+b_1+b_2^-1);
a_2^-1 is inverse of a_2.
If a_2 is 3, then a_2^-1 is 9.
But I inverse of a_2 is one of the keys of affine.
So I just use affine key with 4-for-loops.

At second, I wanted to make a hill cipher function.
So I made a function that generate invertible matrix with  size n.
The generator()function will make a matrix with random number in z_26. And it will check the determinant of this matrix.
If the determinant is one of affine cipher key, which means it just has "1" as gcd with 26, then it return the pointer of matrix.
In generator, I use det() function that return determinant of matrix.
In det(), I use cofactor() function that return the cofactorvalue of (x,y) element.
det() and cofactor() runs recursively.

In hill cipher, I use the fact that A*x1 = B*x2.
x2 = B^-1*A*x1.
Since B is invertible, B is also inverse matrix of B^-1.
So I just generate two matrix not one matrix and inverse of another matrix.
Since there is a lot of invertible matrix, I just repeat 500 times to get pair of (x1,x2).

