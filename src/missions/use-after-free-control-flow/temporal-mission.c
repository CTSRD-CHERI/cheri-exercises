/*
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright (c) 2020 Microsoft, Inc.
 */
#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/queue.h>

#if defined(__CHERI_PURE_CAPABILITY__)
#include <cheri/revoke.h>
static void __attribute__((used)) *check_cheri_revoke = cheri_revoke;
extern void malloc_revoke(void);
static void __attribute__((used)) *check_malloc_revoke = malloc_revoke;
#endif

struct farm;
struct crop;

static void
success(struct farm *f)
{
	fprintf(stderr, "Exploit successful: computer bought the farm!\n");
	exit(42);
}

TAILQ_HEAD(cropq, crop);

struct farm {
	struct cropq	cropq;
	struct crop	*cursor;
};

struct crop {
	void			(*describe)(struct crop *);
	TAILQ_ENTRY(crop)	farm_cropq;
};

_Static_assert(sizeof(struct crop) == sizeof(struct farm),
    "Structure size mismatch");

union ufo {
	void	*ptrA;
	char	buf[sizeof(struct crop)];
};

_Static_assert(sizeof(struct crop) == sizeof(union ufo),
    "Structure size mismatch");

static void
descr_1(struct crop *c)
{
	fprintf(stderr, "Chelan at %p\n", c); /* Pacific Northwest */
}

static void
descr_2(struct crop *c)
{
	fprintf(stderr, "Colt at %p\n", c); /* United Kingdom */
}

static unsigned int cid;
static union ufo *ufo;

static const size_t nfarm = 4;
struct farm *farmp[nfarm];

static void
rm_farm(int fix)
{
	struct farm *f = farmp[fix];

	farmp[fix] = NULL;
	if (f != NULL) {
		struct crop *c, *tc;

		TAILQ_FOREACH_SAFE(c, &f->cropq, farm_cropq, tc) {
			TAILQ_REMOVE(&f->cropq, c, farm_cropq);
			free(c);
		}

		fprintf(stderr, "Tear down farm (index %d) at %p\n", fix, f);
		free(f);
	}
}

static struct farm *
mk_farm(int fix)
{
	struct farm *f;

	rm_farm(fix);

	f = malloc(sizeof(struct farm));
	assert(f != NULL); /* Surely infinite memory */

	TAILQ_INIT(&f->cropq);
	f->cursor = NULL;

	farmp[fix] = f;

	fprintf(stderr, "New farm (index %d) at %p\n", fix, f);
	return f;
}

static void
rm_crop(struct farm *f, struct crop *c)
{
	fprintf(stderr, "Del crop at %p\n", c);
	TAILQ_REMOVE(&f->cropq, c, farm_cropq);
	free(c);
}

static struct crop *
mk_crop(struct farm *f)
{
	struct crop *c;

	c = malloc(sizeof(struct crop));
	assert(c != NULL);

	if (f->cursor != NULL) {
		/* Inherit description of current cursor */
		c->describe = f->cursor->describe;
	} else {
		c->describe = (cid & 1) ? descr_1 : descr_2 ;
	}
	cid++;

	TAILQ_INSERT_HEAD(&f->cropq, c, farm_cropq);

	fprintf(stderr, "New crop at %p\n", c);

	return c;
}

static void
rm_ufo(void)
{
	if (ufo != NULL) {
		fprintf(stderr, "Del UFO at %p\n", ufo);
		free(ufo);
	}
}

int
main(void)
{
	int c;
	size_t fix = 0;

#if defined(__CHERI_PURE_CAPABILITY__)
	if (getenv("MALLOC_DISABLE_REVOCATION") == NULL) {
		fprintf(stderr, "Ready (CHERI-RISC-V)\n");
	} else {
		fprintf(stderr, "Ready (CHERI-RISC-V, reduced heap safety)\n");
	}
#else
	fprintf(stderr, "Ready (RISC-V)\n");
#endif

	while ((c = getchar()) != EOF) {
		if (isspace(c))
			continue;

		if (('0' <= c) && (c < '0' + nfarm)) {
			fix = c - '0';
			fprintf(stderr, "Selected farm %zu (%p)\n", fix,
			    farmp[fix]);
			continue;
		}

		struct farm *f = farmp[fix];
		switch (c) {
		case '!':
#if defined(__CHERI_PURE_CAPABILITY__)
			malloc_revoke();
#else
			fprintf(stderr, "No revocation without CHERI!\n");
#endif
			break;

		/* Crop management */
		case 'C':
			if (f != NULL)
				mk_crop(f);
			break;
		case 'c':
			if ((f != NULL) && (f->cursor != NULL))
				rm_crop(f, f->cursor);
			break;
		case 'D':
			fprintf(stderr, "FYI: Current farm is %p\n", f);
			if ((f != NULL) && (f->cursor != NULL)) {
				fprintf(stderr, "FYI:  cursor %p\n", f->cursor);
				fprintf(stderr, "FYI:  cursor->describe %p\n",
				    f->cursor->describe);
				f->cursor->describe(f->cursor);
			}
			break;

		/* Farm management */
		case 'F':
			mk_farm(fix);
			break;
		case 'f':
			rm_farm(fix);
			break;

		/* Cursor control */
		case 'L':
			if (f != NULL) {
				if (f->cursor != NULL) {
					f->cursor = TAILQ_PREV(f->cursor, cropq,
					    farm_cropq);
				} else {
					f->cursor = TAILQ_LAST(&f->cropq,
					    cropq);
				}
			}
			fprintf(stderr, "Farm %zu cursor %p\n", fix, f->cursor);
			break;
		case 'R':
			if (f != NULL) {
				if (f->cursor != NULL) {
					f->cursor = TAILQ_NEXT(f->cursor,
					    farm_cropq);
				} else {
					f->cursor = TAILQ_FIRST(&f->cropq);
				}
			}
			fprintf(stderr, "Farm %zu cursor %p\n", fix, f->cursor);
			break;
		case 'Z':
			if (f != NULL)
				f->cursor = NULL;
			break;

		/* UFO control sequences */
		case 'A':
			if ((ufo != NULL) && (f != NULL)) {
				fprintf(stderr, "UFO abduct %p\n", f->cursor);
				ufo->ptrA = f->cursor;
			}
			break;
		case 'S':
			if (f != NULL) {
				/* Jess's Organic Farm-to-Vtable Capability */
				fprintf(stderr, "Crop sign at farm %p\n", f);
				f->cursor = (void *)success;
			}
			break;
		case 's':
			if ((f != NULL) && (f->cursor != NULL)) {
				char buf[sizeof(void *)];
				for (size_t i = 0; i < sizeof buf; i++) {
					buf[i] = getchar();
				}
				fprintf(stderr, "Signing crop %p\n", f->cursor);
				memmove((char *)f->cursor, buf, sizeof(buf));
			}
			break;
		case 'U':
			rm_ufo();
			ufo = malloc(sizeof(union ufo));
			assert(ufo != NULL);
			fprintf(stderr, "UFO at %p\n", ufo);
			break;
		case 'u':
			rm_ufo();
			break;

		default:
			fprintf(stderr, "Did not understand %x; bail!\n", c);
			return 1;
		}
	}

	return 0;
}
