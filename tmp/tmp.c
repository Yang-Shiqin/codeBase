#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]){
	int nsecs, opt;

	while ((opt = getopt(argc, argv, "nt:W;")) != -1) {
		switch (opt) {
		case 'n': 
            printf("\nn");
            break;
		case 't':
		    nsecs = atoi(optarg);
            printf("\n%d", nsecs);
		    break;
        case 'W':
            printf("\n%s", optarg);
		default: break; /* '?' */
	    }
	}
    return 0;
}
