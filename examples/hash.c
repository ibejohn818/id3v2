#include <stdio.h>
#include "sodium.h"


  #define MESSAGE ((const unsigned char *) "Arbitrary data to hash")
#define MESSAGE_LEN 22

int main(void) {


unsigned char hash[crypto_generichash_BYTES];

crypto_generichash(hash, sizeof hash,
                   MESSAGE, MESSAGE_LEN,
                   NULL, 0);

  printf("HASH: %s", hash);

  for(uint32_t i=0; i < crypto_generichash_BYTES; i++) {
    printf("%X ", hash[i]);
  }
  printf("\n");

  return 0;
}
