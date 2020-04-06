#include <iostream>
#include <fstream>

//copied from http://www.codecodex.com/wiki/Read_a_file_into_a_byte_array
char* readFileBytes(const char *name)
{
    ifstream fl(name);
    fl.seekg( 0, ios::end );
    size_t len = fl.tellg();
    char *ret = new char[len];
    fl.seekg(0, ios::beg);
    fl.read(ret, len);
    fl.close();
    return ret;
}
