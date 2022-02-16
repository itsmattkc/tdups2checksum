#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint32_t table[256];

uint32_t Checksum(uint32_t input, uint32_t previous)
{
  uint32_t v0 = previous >> 24;
  uint32_t v1 = previous << 8;
  v0 ^= input;
  v0 &= 0xFF;
  v0 = table[v0];
  return v1 ^ v0;
}

int main(int argc, char **argv)
{
  if (argc != 2) {
    printf("Usage: %s <TDU-save>\n", argv[0]);
    return 1;
  }

  // Open file
  FILE *f = fopen(argv[1], "rb");
  if (!f) {
    printf("Failed to load \"%s\"\n", argv[1]);
    return 1;
  }

  // Read checksum from file
  uint32_t file_checksum;
  fread(&file_checksum, sizeof(uint32_t), 1, f);

  // Generate table
  for (int i=0; i<256; i++) {
    uint32_t sum = i << 24;

    for (int j=0; j<8; j++) {
      sum = (sum * 2) ^ ((sum & 0x80000000) ? 79764919 : 0);
    }

    table[i] = sum;
  }

  // Generate checksum
  uint32_t checksum = 0;

  // Read data
  size_t read_sz;
  do {
    uint32_t i = 0;
    read_sz = fread(&i, 1, sizeof(uint32_t), f);

    if (read_sz < sizeof(uint32_t)) {
      // Ending bytes are read differently, they're read and bitshifted one by one into a uint32
      // like so.
      uint8_t *b = (uint8_t *) &i;
      uint32_t j = 0;
      for (size_t k=0; k<read_sz; k++) {
        j <<= 8;
        j |= b[k];
      }
      i = j;
    }

    checksum = Checksum(i, checksum);
  } while (read_sz == sizeof(uint32_t));

  fclose(f);

  // Print
  printf("%u\n", checksum);

  return 0;
}
