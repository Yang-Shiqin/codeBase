long plus(long x, long y);

void sumstore(long x, long y, long *dst){
    long t = plus(x, y);
    *dst = t;
}
