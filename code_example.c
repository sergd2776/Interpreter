int factorial(int n){
    if (n == 1){
        return 1;
    }
    else if (n > 1){
        return n * factorial(n-1);
    }
    else {
        return 0;
    }
}

void Change(int* a){
    *a = factorial(6);
}
int main(){
    int a;
    Change(&a);
    int i = 0;
    while (1){
    if (i == 4){
        break;
    }
    else if (i % 2 == 1){
        i++;
        continue;
    }
        a += a;
        i++;
    }
    printf(a);
    return 0;
}
/*
int main(){
    int* a;
    malloc(&a, 15);
    a[1] = 4;
    return a[1];
}*/
