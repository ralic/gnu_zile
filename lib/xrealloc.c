/* $Id: xrealloc.c,v 1.6 2004/03/10 12:59:32 rrt Exp $ */

#ifndef HAVE_XREALLOC

#include <stdio.h>
#include <stdlib.h>

#include "config.h"

/*
 * Resize an allocated memory area.
 */
void *(xrealloc)(void *ptr, size_t size)
{
	void *newptr;

	if ((newptr = realloc(ptr, size)) == NULL) {
		fprintf(stderr, "zile: cannot reallocate memory\n");
		exit(1);
	}

	return newptr;
}

#endif
