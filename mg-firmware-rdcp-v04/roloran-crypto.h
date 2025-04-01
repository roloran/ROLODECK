#ifndef _ROLORAN_CRYPTO
#define _ROLROAN_CRYPTO

#include <GCM.h>
#include <Crypto.h>
#include <AES.h>

#include <Arduino.h>
#include <SchnorrSigCtx.h>
#include <SchnorrSigSign.h>
#include <SchnorrSigVerify.h>
#include <SHA256.h>

/**
* Encrypts data with AES-256-GCM and writes output into out_ciphertext and out_tag. On success, returns true.
* @param plaintext Plaintext to encrypt
* @param ptsize Size of plaintext in number of bytes
* @param adata Additional data for authentication
* @param adatasize Size of additional data in number of bytes
* @param key AES-256 key used for encryption
* @param keysize Encryption key size in bytes (must be 32)
* @param iv Initialization Vector (IV) to use for AES-GCM
* @param ivsize IV size in number of bytes (must be 12)
* @param out_ciphertext Buffer for resulting ciphertext, must be at least same size as plaintext
* @param out_tag Buffer for resulting AES-GCM AuthTag, must be 16 bytes in size
* @param tagsize Size of the authentication tag in number of bytes (must be 16)
* @return true on success, otherwise false
*/
bool encrypt_aes256gcm(uint8_t *plaintext, size_t ptsize, uint8_t *adata, size_t adatasize, uint8_t *key, size_t keysize, uint8_t *iv, size_t ivsize, uint8_t *out_ciphertext, uint8_t *out_tag, size_t tagsize);

/**
* Decrypts ciphertext with AES-256-GCM and writes output into out_plaintext. 
* On success and if data is authenticated, returns true, otherwise false.
* @param ciphertext Ciphertext to decrypt, excluding the GCM AuthTag
* @param csize Size of ciphertext in bytes 
* @param adata Additional data used for AEAD, must be same as used when encrypting
* @param adatasize Size of adata in bytes
* @param key AES-256 key, must be same as used for encryption
* @param keysize Size of key in bytes (must be 32)
* @param iv Initialization Vector, must be same as used for encryption
* @param ivsize IV size in bytes (must be 12)
* @param tag AES-GCM AuthTag as received along with the ciphertext
* @param tagsize Size of the AuthTag (must be 16)
* @param out_plaintext Buffer for storing the resulting plaintext
* @return true if message was authenticated and successfully decrypted, false if either step went wrong
*/
bool decrypt_aes256gcm(uint8_t *ciphertext, size_t csize, uint8_t *adata, size_t adatasize, uint8_t *key, size_t keysize, uint8_t *iv, size_t ivsize, uint8_t *tag, size_t tagsize, uint8_t *out_plaintext);

/**
* Simple AES-256-GCM function with as many fixed params as possible.
* Encrypts data with AES-256-GCM with Key of length 32 and IV/Nonce of length 12 (!!) and writes 
* output into out_ciphertext and out_tag. On success, returns num of bytes of ciphertext 
* (16 BYTES TAG EXCLUDED), otherwise false.
*/
bool encrypt_aes256gcm_simplified(uint8_t *plaintext, size_t ptsize, uint8_t *adata, size_t adatasize, uint8_t *key, uint8_t *iv, uint8_t *out_ciphertext, uint8_t *out_tag);

/**
* Simple AES-256-GCM function with as many fixed params as possible.
* Decrypts ciphertext with AES-256-GCM and writes output into out_plaintext. 
* Key AND Tag must be of length 16 and IV/Nonce must be of 12 bytes length! On success and if data 
* is authenticated, returns true, otherwise false. 
*/
bool decrypt_aes256gcm_simplified(uint8_t *ciphertext, size_t csize, uint8_t *adata, size_t adatasize, uint8_t *key, uint8_t *iv, uint8_t *tag, uint8_t *out_plaintext);


/**
 * Creates a Schnorr signature for the given data whose length must also be specified. 
 * The targetbuffer must hold at least the 65 bytes for the signature. 
 * Returns the length of the signature in bytes, usually 65.
 * The signed data can be of arbitrary length and content, as it is hashed internally;
 * however, also, for example, a 32-byte SHA-256 has can be used as input.
 * Uses the Schnorr Private Key stored on the device and returns -1 in case of error. 
 * @param data Data to be signed, e.g., a string or a hash value
 * @param datalen Length of data in bytes, e.g., 32 if the data is a 256-bit hash value
 * @param targetbuffer Buffer to hold the resulting Schnorr signature
 * @return Length of the created Schnorr signature, usually 65
 */
int schnorr_create_signature(uint8_t *data, uint8_t datalen, uint8_t *targetbuffer);

/**
 * Verifies a Schnorr signature using the HQ's Public Key stored on the device. 
 * data and datalen must be given just like for the creation of a signature. 
 * The given signature is typically received over-the-air and expected to be 65 bytes long,
 * where the first 33 bytes are for the Point and the remaining 32 bytes for the Signature itself. 
 * Returns true if the signature is valid, or false otherwise. 
 * @param data Data for which the signature shall be verified
 * @param datalen Length of data in bytes
 * @param signature 65-byte Schnorr signature used for verification
 * @return true if data and signature match, false if data is not authentic
 */
bool schnorr_verify_signature(uint8_t *data, uint8_t datalen, uint8_t *signature);

#endif
// EOF