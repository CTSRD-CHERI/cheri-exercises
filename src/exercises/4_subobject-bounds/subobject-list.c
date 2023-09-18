/*
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright (c) 2022 Microsoft Corporation
 *
 * This exercise investigates a circular doubly-linked list with sentinels.
 */
#include <stdio.h>

/*
 * A list element is an intrusive structure (subobject) with a pointer to the
 * next list element and a pointer to the previous node's next pointer.  In the
 * case of an empty list, ile_prevnp points to ile_next.
 */
struct ilist_elem {
	struct ilist_elem **ile_prevnp;
	struct ilist_elem *ile_next;
}; // __subobject_use_container_bounds;

static void
ilist_init_sentinel(struct ilist_elem *s) {
	s->ile_next = s;
	s->ile_prevnp = &s->ile_next;
}

static void
ilist_insert_after(struct ilist_elem *p, struct ilist_elem *n) {
	n->ile_next = p->ile_next;
	p->ile_next = n;
	n->ile_next->ile_prevnp = &n->ile_next;
	n->ile_prevnp = &p->ile_next;
}

static void
ilist_remove(struct ilist_elem *e) {
	e->ile_next->ile_prevnp = e->ile_prevnp;
	*(e->ile_prevnp) = e->ile_next;
}

#define ILIST_FOREACH(h, c) \
	for(c = (h)->ile_next; c != (h); c = c->ile_next)

#ifdef USE_CDEFS_CONTAINEROF
#define ILIST_CONTAINER(elem, type, field) \
	(((elem) == NULL) ? ((type *)NULL) : __containerof((elem), type, field))
#else
#define ILIST_CONTAINER(elem, type, field) \
	(((elem) == NULL) ? ((type *)NULL) : \
		__DEQUALIFY(type*, (const volatile char *)(elem) - \
			__offsetof(type, field)))
#endif

struct obj {
	int val;
	struct ilist_elem ilist; // __subobject_use_container_bounds;
};

struct ilist_elem l; /* Sentinel element serves as list head */
struct obj obj1 = {1, {}};
struct obj obj2 = {2, {}};
struct obj obj3 = {3, {}};

int
main() {
	struct ilist_elem *cursor;

	ilist_init_sentinel(&l);
	ilist_insert_after(&l, &obj2.ilist);
	ilist_insert_after(&obj2.ilist, &obj3.ilist);
	ilist_insert_after(&l, &obj1.ilist);
	ilist_remove(&obj2.ilist);

	printf("Traversing list=%#p first=%#p lastnp=%#p\n",
		&l, l.ile_next, l.ile_prevnp);
	ILIST_FOREACH(&l, cursor) {
		struct obj *co = ILIST_CONTAINER(cursor, struct obj, ilist);
		printf(" Ilist cursor=%#p\n  next=%#p\n  prevnp=%#p\n",
			cursor, cursor->ile_next, cursor->ile_prevnp);
		printf("  val field at %#p\n",
			/*
			 * This ugly bit of syntax is unfortunate, but avoids
			 * a subobject-bounds-induced trap that isn't the first
			 * one you should think about.  I'm sorry.  Just pretend
			 * this says "&co->val" and, for extra credit, later,
			 * explain why it isn't spelled like that.
			 */
			((char *)co) + __offsetof(struct obj, val));
	}

	printf("Traversing list again, accessing superobject field...\n");
	ILIST_FOREACH(&l, cursor) {
		struct obj *co = ILIST_CONTAINER(cursor, struct obj, ilist);
		printf(" Ilist cursor=%#p value=%d (at %#p)\n", cursor,
			co->val, &co->val);
	}
}
