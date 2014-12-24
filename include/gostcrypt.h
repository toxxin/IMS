/*
 * The GOST 28147-89 cipher
 *
 * This is based on the 25 Movember 1993 draft translation
 * by Aleksandr Malchik, with Whitfield Diffie, of the Government
 * Standard of the U.S.S.R. GOST 28149-89, "Cryptographic Transformation
 * Algorithm", effective 1 July 1990.  (Whitfield.Diffie@eng.sun.com)
 *
 * That is a draft, and may contain errors, which will be faithfully
 * reflected here, along with possible exciting new bugs.
 *
 * Some details have been cleared up by the paper "Soviet Encryption
 * Algorithm" by Josef Pieprzyk and Leonid Tombak of the University
 * of Wollongong, New South Wales.  (josef/leo@cs.adfa.oz.au)
 *
 * The standard is written by A. Zabotin (project leader), G.P. Glazkov,
 * and V.B. Isaeva.  It was accepted and introduced into use by the
 * action of the State Standards Committee of the USSR on 2 June 89 as
 * No. 1409.  It was to be reviewed in 1993, but whether anyone wishes
 * to take on this obligation from the USSR is questionable.
 *
 * This code is placed in the public domain.
 */

/*
 * If you read the standard, it belabors the point of copying corresponding
 * bits from point A to point B quite a bit.  It helps to understand that
 * the standard is uniformly little-endian, although it numbers bits from
 * 1 rather than 0, so bit n has value 2^(n-1).  The least significant bit
 * of the 32-bit words that are manipulated in the algorithm is the first,
 * lowest-numbered, in the bit string.
 */

#ifndef GOSTCRYPT_H_
#define GOSTCRYPT_H_

void kboxinit(void);

void gostcrypt(uint32_t const in[2], uint32_t out[2], uint32_t const key[8]);

void gostdecrypt(uint32_t const in[2], uint32_t out[2], uint32_t const key[8]);


#endif /* GOSTCRYPT_H_ */
