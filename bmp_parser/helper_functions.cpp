#include <iostream>
#include <fstream>
#include <cstdint>
#include <stdio.h> //for FILE and fopen

#include "debug_settings.h"

//copied from http://www.codecodex.com/wiki/Read_a_file_into_a_byte_array
char* readFileBytes(const char *name)
{
    std::ifstream fl(name);
    fl.seekg( 0, std::ios::end );
    size_t len = fl.tellg();
    char *ret = new char[len];
    fl.seekg(0, std::ios::beg);
    fl.read(ret, len);
    fl.close();
    return ret;
}

void writeFileBytes(const char* name, uint8_t* bytes, uint32_t len){
    FILE* file_ptr;
    file_ptr = fopen(name, "wb");
    if(file_ptr != nullptr){
        uint32_t written = fwrite(bytes, sizeof(uint8_t), len, file_ptr);
        fclose(file_ptr);
        if(written != len){
            if(DEBUG_LEVEL > 0) printf("Error, byte array length (%d bytes) and written bytes (%d bytes) do not match\n", len, written);
        }
    }
}

int constructIntegerFromByteArray(char* addr, int numBytes, bool littleEndian){
    int sum = 0;
    if(littleEndian){
        int mult = 1;
        int i;
        for(i = 0; i < numBytes; i++){
            uint8_t value = (uint8_t) addr[i];
            sum += (value * mult);
            mult *= 256;
        }
    }
    else{
        int mult = 1;
        int i;
        for(i = numBytes - 1; i >= 0; i--){
            sum += (addr[i] * mult);
            mult *= 256;
        }
    }

    return sum;
}
