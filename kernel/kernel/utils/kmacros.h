#ifndef KMACROS_H
#define KMACROS_H

/*
 * Here is some basic general-purpose support macros:
*/

#define TO_STRING( X ) #X

#define __CONCAT_IMPL_2( A, B ) A##B
#define MACRO_CONCAT_2( A, B ) __CONCAT_IMPL_2( A, B )

#endif // KMACROS_H
