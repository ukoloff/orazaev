#include<stdio.h>

int strlen(const char * str) {
    int ans = 0;
    while(*str++) 
        ++ans;
    return ans;
}

char * strcat(char * destination, const char * source) {
    char * ans = destination;
    int len = strlen(destination);
    while(*(destination + len) = *source++)
        ++destination;
    *(destination + len) = 0;
    
    return ans;
}

char * strrev(char * destination, const char * source) {
    char * ans = destination;
    int len = strlen(source);
    while(*destination++ = *(source + len - 1))
        --source;
    *destination = 0;

    return ans;
}

char * itoa (unsigned int value, char * str, unsigned int base) {
    char * ans = str;
    if (value == 0) {
        *str++ = '0';
        *str = 0;
        return ans;
    }
    
    while(value) {
        int number = value % base;

        if (number >= 10)
            *str++ = 'A' + number - 10;
        else
            *str++ = '0' + number;
        value -= number;
        value /= base;
    }

    *str = 0;
    char * temp = new char[strlen(ans) + 1];
    *temp = 0;

    strcat(temp, ans);
    strrev(ans, temp);

    delete[] temp;
    return ans;
}

int main() {
    char c[200];
    
    printf("%s\n", itoa(7, c, 2));
    printf("%s\n", itoa(7, c, 16));
    printf("%s\n", itoa(25, c, 16));
    printf("%s\n", itoa(440, c, 16));
    printf("%s\n", itoa(0, c, 16));
}
