int printf(const char* x, ...);

int foo(int x);

int main(){
    printf("%d\n", foo(5));
    return 0;
}
