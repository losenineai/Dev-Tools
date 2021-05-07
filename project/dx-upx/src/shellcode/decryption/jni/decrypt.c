
extern cacheflush(char* addr, int nbytes, int cache);


int main(){
    cacheflush(0, 0, 0);
    return 0;
}

