#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "md5.h"

unsigned char data[] = {
    0xfa, 0xa0, 0x3e, 0xe8, 0xf5, 0x19, 0xde, 0x4e,
    0xac, 0x9c, 0x9c, 0xfc, 0x33, 0x6a, 0x4d, 0xae,
    0xd6, 0xe3, 0x28, 0xe4, 0xe6, 0x5c, 0xc3, 0x01,
    0xb6, 0xc9, 0x86, 0xbd, 0x28, 0x6a, 0x1e, 0xaf,
    0xd2, 0xe7, 0x28, 0xa1, 0xb0, 0x5e, 0xc8, 0x4e,
    0xbd, 0x92, 0xd4, 0xc9, 0x2d, 0x6a, 0x4d, 0xaf,
    0xd6, 0xe3, 0x3f, 0xe8, 0xe4, 0x19, 0xd7, 0x49,
    0xab, 0xdd, 0x87, 0xf8, 0x65, 0x66, 0x1e, 0xe6,
    0x93, 0xd3, 0x34, 0xfe, 0xe4, 0x5c, 0xca, 0x01,
    0x9d, 0xce, 0x81, 0xf4, 0x21, 0x0f,
};

bool check_password(char *password);
void decrypt(unsigned char *data, size_t sz, char *password);
void derive_key(char *password, unsigned char *output_key);

int main() {
  char password[64] = {};
  puts("Secret Safe with Secret Data");

  printf("Password plz: ");
  fflush(stdout);

  if (scanf("%63s", password) != 1) {
    puts("No? Well, bye.");
    return 1;
  }

  if (!check_password(password)) {
    puts("Wrong password!");
    return 2;
  }

  puts("Here's your data:");

  decrypt(data, sizeof(data), password);
  puts((char*)data);  // Decrypted data is NUL-terminated.

  return 0;
}

bool check_password(char *password) {
  if (strlen(password) != 41) {
    return false;
  }

  MD5_CTX md5;
  MD5_Init(&md5);
  MD5_Update(&md5, password, strlen(password));

  unsigned char result[16];
  MD5_Final(result, &md5);

  if (memcmp(result, "\x57\xd9\xb1\xfd\x25\x52\xff\x0b\x8e\x5a\xeb\x18\x75\x4a\x9b\x03", 16) != 0) {
    return false;
  }

  return true;
}

void decrypt(unsigned char *data, size_t sz, char *password) {
  unsigned char result[16];
  derive_key(password, result);

  for (size_t i = 0; i < sz; i++) {
    data[i] ^= result[i % 16];
  }
}


void derive_key(char *password, unsigned char *output_key) {
  uint64_t key = 0xf8a45191c23a75be;
  for (int i = 0; password[i] != '\0'; i++) {
    key += password[i];
  }

  MD5_CTX md5;
  MD5_Init(&md5);
  MD5_Update(&md5, &key, 8);
  MD5_Final(output_key, &md5);
}

