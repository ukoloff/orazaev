#include<cstring>
#include<string>
#include<cstdio>
#include<iostream>

const int buf_size = 128;

int main(int argc, char* argv[]) {
    if (argc < 2) return 1;
    FILE* f;
    
    f = fopen(argv[1], "rb");
    
    if (f == 0) return 2;
    
    char buf[buf_size];

    while(!feof(f)) {
        int i;
        fread(buf, sizeof(char)*buf_size, 1, f);
        std::cout << buf << " ";
        fread(&i, sizeof(int), 1, f);
        std::cout << i << std::endl;
    }
    
    fclose(f);

    
    return 0;
}
