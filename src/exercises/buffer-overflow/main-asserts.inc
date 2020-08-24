#include <assert.h>
#include <stddef.h>
#ifdef __CHERI_PURE_CAPABILITY__
#include <cheriintrin.h>
#endif

#ifndef nitems
#define	nitems(x)	(sizeof((x)) / sizeof((x)[0]))
#endif

static void
main_asserts(void)
{
	assert((ptraddr_t)&buffer[nitems(buffer)] == (ptraddr_t)&c
#ifdef __CHERI_PURE_CAPABILITY__
	    || sizeof(buffer) < cheri_representable_length(sizeof(buffer))
#endif
	    );
}
