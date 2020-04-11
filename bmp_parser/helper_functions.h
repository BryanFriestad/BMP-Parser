#ifndef HELPER_FUNCTIONS_H_INCLUDED
#define HELPER_FUNCTIONS_H_INCLUDED

#include <cstdint>

char* readFileBytes(const char *name);
void writeFileBytes(const char* name, uint8_t* bytes, uint32_t len);
int constructIntegerFromByteArray(char* addr, int numBytes, bool littleEndian);

#endif // HELPER_FUNCTIONS_H_INCLUDED
