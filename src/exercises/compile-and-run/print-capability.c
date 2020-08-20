#include <stdio.h>
#include <cheriintrin.h>

int
main(void)
{
	int i;
	void *cap = &i;

	printf("cap length: %lu\n", cheri_length_get(cap));

	return (0);
}
