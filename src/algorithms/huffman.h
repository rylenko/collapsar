#ifndef _HUFFMAN_H
#define _HUFFMAN_H

#ifdef __cplusplus
#include <cstddef>

extern "C" {
#else
#include <stddef.h>
#endif /* __cplusplus */

/*
 * Compresses passed data using the Huffman's coding algorithm.
 */
void huffman_compress(unsigned char *, size_t);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _HUFFMAN_H */
