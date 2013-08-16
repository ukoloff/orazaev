void swap(int& a, int& b) {
    int c = b;
    b = a;
    a = c;
}

int main() {
    // swap(0, 1); не компилируется
}
