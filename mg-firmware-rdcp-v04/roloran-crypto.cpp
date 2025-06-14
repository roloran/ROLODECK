#include "roloran-crypto.h"
#include "roloran-tdeck-serial.h"
#include "settings-device.h"
#include "settings-scenario.h"

bool encrypt_aes256gcm(uint8_t *plaintext, size_t ptsize, uint8_t *adata, size_t adatasize, uint8_t *key, size_t keysize, uint8_t *iv, size_t ivsize, uint8_t *out_ciphertext, uint8_t *out_tag, size_t tagsize) 
{
  GCM<AES256> gcm;
  gcm.setKey(key, keysize);
  gcm.setIV(iv, ivsize);
  gcm.addAuthData(adata, adatasize);
  gcm.encrypt(out_ciphertext, plaintext, ptsize);
  gcm.computeTag(out_tag, tagsize);
  return true;
}

bool decrypt_aes256gcm(uint8_t *ciphertext, size_t csize, uint8_t *adata, size_t adatasize, uint8_t *key, size_t keysize, uint8_t *iv, size_t ivsize, uint8_t *tag, size_t tagsize, uint8_t *out_plaintext) 
{
  GCM<AES256> gcm;
  gcm.setKey(key, keysize);
  gcm.setIV(iv, ivsize);
  gcm.addAuthData(adata, adatasize);
  gcm.decrypt(out_plaintext, ciphertext, csize);
  if (!gcm.checkTag(tag, tagsize)) 
  {
      return false;
  }
  return true;
}

bool encrypt_aes256gcm_simplified(uint8_t *plaintext, size_t ptsize, uint8_t *adata, size_t adatasize, uint8_t *key, uint8_t *iv, uint8_t *out_ciphertext, uint8_t *out_tag) 
{
  return encrypt_aes256gcm(plaintext, ptsize, adata, adatasize, key, 32, iv, 12, out_ciphertext, out_tag, 16);
}

bool decrypt_aes256gcm_simplified(uint8_t *ciphertext, size_t csize, uint8_t *adata, size_t adatasize, uint8_t *key, uint8_t *iv, uint8_t *tag, uint8_t *out_plaintext) 
{
  return decrypt_aes256gcm(ciphertext, csize, adata, adatasize, key, 32, iv, 12, tag, 16, out_plaintext);
}

SchnorrSigCtx ssc;
bool ssc_initialized = false;
SchnorrSigVerify ssv;
bool ssv_initialized = false;


bool schnorr_init_ctx(void)
{
  if (ssc_initialized) return true;

  /*
  ssc = SchnorrSigCtx();
  int res = ssc.init();
  */
  size_t seed_len = MBEDTLS_CTR_DRBG_MAX_SEED_INPUT - MBEDTLS_CTR_DRBG_ENTROPY_LEN;
  uint8_t seed[seed_len];    
  esp_fill_random(seed, seed_len);

  ssc = SchnorrSigCtx();
  int res = ssc.init(seed, seed_len);
  if (res != 0)
  {
    serial_writeln("ERROR: schnorr_init_ctx() failed");
    return false;
  }

  ssc_initialized = true;
  return true;
}

int schnorr_create_signature(uint8_t *data, uint8_t datalen, uint8_t *targetbuffer)
{
  schnorr_init_ctx();

  SchnorrSigSign sss = SchnorrSigSign();
  char *myprivkey = getMyPrivateKey();
  int res = sss.init(&ssc, myprivkey);
  if (res != 0)
  {
    char buf[INFOLEN];
    snprintf(buf, INFOLEN, "ERROR: schnorr_create_signature() could not initialize with private key %s", myprivkey);
    serial_writeln(buf);
    return -1;
  }

  struct SchnorrSigCtx::signature new_sig;
  res = sss.sign((const unsigned char*)data, datalen, &new_sig);
  if (res != 0)
  {
    serial_writeln("ERROR: schnorr_create_signature() signing failed");
    return -1;
  }

  int tbi = 0;
  for (int i=0; i < new_sig.point_len; i++)
  {
    targetbuffer[tbi++] = new_sig.point[i];
  }
  for (int i=0; i < new_sig.sig_len; i++)
  {
    targetbuffer[tbi++] = new_sig.sig[i];
  }

  return tbi;
}

int schnorr_init_verify(void)
{
  if (ssv_initialized) return 0;
  ssv = SchnorrSigVerify();
  char *HQPubKey = getHQpublicKey();
  int res = ssv.init(&ssc, HQPubKey);
  ssv_initialized = true;
  return res;
}

bool schnorr_verify_signature(uint8_t *data, uint8_t datalen, uint8_t *signature)
{
  schnorr_init_ctx();
  int res = schnorr_init_verify();
                               
  if (res != 0)
  {
    char msg[INFOLEN];
    snprintf(msg, INFOLEN, "ERROR: schnorr_verify_signature() could not initialize (res %d) with HQ public key %s", res, getHQpublicKey());
    serial_writeln(msg);
    return false;
  }

  struct SchnorrSigCtx::signature sig;
  sig.point_len = SCHNORRPOINTLEN;
  for (int i=0; i<SCHNORRPOINTLEN; i++) sig.point[i] = signature[i];
  sig.sig_len = SCHNORRSIGLEN;
  for (int i=0; i<SCHNORRSIGLEN; i++) sig.sig[i] = signature[i+SCHNORRPOINTLEN];

  res = ssv.verify((const unsigned char*)data, datalen, &sig);
  if (res == 0) return true;

  return false;
}

/* EOF */