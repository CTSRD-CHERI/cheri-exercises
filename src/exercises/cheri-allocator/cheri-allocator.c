/*
 * SPDX-License-Identifier: BSD-2-Clause-DARPA-SSITH-ECATS-HR0011-18-C-0016
 * Copyright (c) 2022 Robert N. M. Watson
 */

#include <sys/cdefs.h>

#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#ifdef __CHERI_PURE_CAPABILITY__
#include <cheriintrin.h>
#endif

/*
 * Implement a very simple allocator for a fixed-size data type, with inline
 * metadata.  Calls to alloc_allocate() return a pointer to a fixed-size byte
 * array.  Calls to alloc_free() return it to the allocator for reuse.
 *
 * The implementation is simplistic, and is designed to support an exercise
 * relating to: (a) bounds setting; and (b) monotonicty and rederivation.
 * Each allocation is described by 'struct allocation', which consists of
 * free-list pointers and an array of bytes that make up the allocation
 * itself.  Those allocations are stored as a sequential array in a global
 * variable initialised by BSS:
 *
 *  /--------- index 0 ----------\ /--------- index 1 ----------\ /--...
 *
 * +--------+-----------------...-+--------+-----------------...-+---...
 * | a_next | a_bytes[ALLOC_SIZE] | a_next | a_bytes[ALLOC_SIZE] |
 * +--------+-----------------...-+--------+-----------------...-+---...
 *
 *                                ^                              ^
 *      \_________________________/    \_________________________/
 *        If unallocated, pointer        If unallocated, pointer
 *        to next free allocation.       to next free allocation.
 *
 * Allocation storage is sized below the threshold requiring extra alignment
 * or padding to account for capability bounds compression.
 */
#define	ALLOC_SIZE		128		/* Allocation data size. */
struct alloc_storage {
	struct alloc_storage	*a_next;		/* Free list. */
	uint8_t			 a_bytes[ALLOC_SIZE];	/* Allocated memory. */
};

#define	ALLOC_MAX	16			/* Availaable allocations. */
struct alloc_storage alloc_array[ALLOC_MAX];	/* Underlying storage. */
struct alloc_storage *alloc_nextfree;		/* Next available memory. */

/*
 * Initialise the free list, pointing alloc_nextfree at the array, and then
 * chaining array entries into the list.
 */
static void
alloc_init(void)
{
	int i;

	alloc_nextfree = &alloc_array[0];
	for (i = 0; i < ALLOC_MAX - 1; i++)
		alloc_array[i].a_next = &alloc_array[i + 1];
	alloc_array[ALLOC_MAX - 1].a_next = NULL;
	assert(alloc_array[ALLOC_MAX - 1].a_next == NULL);
}

/*
 * Allocate memory, pulling it off the free list and updating pointers as
 * needed.
 */
static void *
alloc_allocate(void)
{
	struct alloc_storage *alloc;

	if (alloc_nextfree == NULL)
		return (NULL);
	alloc = alloc_nextfree;
	alloc_nextfree = alloc->a_next;
	alloc->a_next = NULL;

	/* Return pointer to allocated memory. */
	return (alloc->a_bytes);
};

/*
 * Free memory, inserting it back into the free list.  Note use of
 * __containerof() to convert pointer to a_bytes back into the container
 * struct pointer.
 */
static void
alloc_free(void *ptr)
{
	struct alloc_storage *alloc;

	/* Convert pointer to allocated memory into pointer to metadata. */
	alloc = __containerof(ptr, struct alloc_storage, a_bytes);
	alloc->a_next = alloc_nextfree;
	alloc_nextfree = alloc;
}

int
main(void)
{
	void *ptr1, *ptr2, *ptr3;

	/* Initialise allocator. */
	alloc_init();
	printf("Allocator initialised\n");

	/*
	 * Allocate some memory.
	 */
	printf("Allocating memory\n");
	ptr1 = alloc_allocate();
	printf("Allocation returned %p\n", ptr1);

	/*
	 * Run off the end of the memory allocation, corrupting the next
	 * allocation's metadata.  Free when done.
	 */
	printf("Preparing to overflow %p\n", ptr1);
	memset(ptr1 + ALLOC_SIZE, 'A', sizeof(void *));
	printf("Overflowed allocation %p\n", ptr1);

	printf("Freeing allocation %p\n", ptr1);
	alloc_free(ptr1);
	printf("Allocation %p freed\n", ptr1);

	/*
	 * Perform three sequential allocations to cause the allocator to
	 * dereference the corrupted pointer, performing a store.
	 */
	printf("Allocating memory\n");
	ptr1 = alloc_allocate();
	printf("Allocation returned %p\n", ptr1);

	printf("Allocating memory\n");
	ptr2 = alloc_allocate();
	printf("Allocation returned %p\n", ptr2);

	printf("Allocating memory\n");
	ptr3 = alloc_allocate();
	printf("Allocation returned %p\n", ptr3);

	/*
	 * Clear up the mess.
	 */
	printf("Freeing allocation %p\n", ptr3);
	alloc_free(ptr3);
	printf("Allocation %p freed\n", ptr3);

	printf("Freeing allocation %p\n", ptr2);
	alloc_free(ptr2);
	printf("Allocation %p freed\n", ptr2);

	printf("Freeing allocation %p\n", ptr1);
	alloc_free(ptr1);
	printf("Allocation %p freed\n", ptr1);

	exit(EX_OK);
}
