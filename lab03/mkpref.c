// mkprefix.c ... generate values in 1..max in prefix order

#include <stdio.h>
#include <stdlib.h>

static void gen (int, int);

int main (int argc, char *argv[])
{
	int max = (argc < 2) ? 63 : atoi (argv[1]);
	gen (1, max);

	return EXIT_SUCCESS;
}

// recursive function to produce prefix order
// should look familiar from e.g. binary search
static void gen (int lo, int hi)
{
	if (lo > hi) return;
	int mid = (lo + hi) / 2;
	printf ("%d\n", mid);
	gen (lo, mid - 1);
	gen (mid + 1, hi);
}
