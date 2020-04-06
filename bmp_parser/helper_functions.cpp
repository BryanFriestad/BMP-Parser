#include <iostream>
#include <fstream>
#include <cstdint>

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

void writeFileBytes(const char* name, uint8_t* bytes, int len){
    std::ofstream file(name);
    if(file.is_open()){
        file.seekp(0);
        file.write((const char*) bytes, len);
        file.close();
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
