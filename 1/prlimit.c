#include <stdio.h>
#include <unistd.h>
#include <limits.h>

int main(int argc, char *argv[]) {
	printf("PATH_MAX : %d | NAME_MAX: %d\n", PATH_MAX, NAME_MAX);
	return 0;
}
