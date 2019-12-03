// tlab.c ... Tree Lab main program
// Builds trees, collects stats
// Written by John Shepherd, March 2013

#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>

#include "Tree.h"

// all values are in range 0000..9999
#define RANGE 10000

static void usage (void) __attribute__((noreturn));
static void mkprefix (int *, int, int, int);
static void mkuniq (int *, int);
static int *makeValues (int, char, int);
static Tree makeTree (int *, int, char);
static void runTests (Tree, int *, int, char);

static int ix = 0; // used by mkprefix()

int main (int argc, char *argv[])
{
	if (! (2 <= argc && argc <= 5)) usage();

	// number of nodes in the tree
	int N = atoi (argv[1]);
	if (N < 0 || N >= RANGE) usage ();

	// order in which values are inserted
	char order = argc >= 3 ? argv[2][0] : 'P';

	// style of insertion
	char style = argc >= 4 ? argv[3][0] : 'L';

	// random number seed
	int seed = argc >= 5 ? atoi (argv[4]) : 1234;

	int *values = makeValues (N, order, seed);

	Tree mytree = makeTree (values, N, style);
	if (TreeNumNodes (mytree) != N)
		errx (
			EX_SOFTWARE,
			"tree not built correctly (got %d nodes, expected %d)",
			TreeNumNodes (mytree), N
		);


	printf ("Tree:\n");
	showTree (mytree);

	char line[BUFSIZ];
	printf ("\n> ");
	while (fgets (line, BUFSIZ, stdin) != NULL) {
		bool show = true;
		int value = atoi (&line[1]);
		switch (line[0]) {
		case 'i':
			TreeInsert (mytree, value);
			break;

		case 'd':
			TreeDelete (mytree, value);
			break;

		case 'f':
			if (TreeFind (mytree, value))
				printf ("Found!\n");
			else
				printf ("Not found\n");
			break;

		case 's':
			// nothing to do ... it's displayed below
			break;

		case 'v':
			for (int i = 0; i < N; i++) {
				if (i % 15 == 0)
					printf ("\n");
				printf ("%d ", values[i]);
			}
			printf ("\n");
			break;

		case 't':
			runTests (mytree, values, N, order);
			break;

		case 'q':
			// handled below
			break;

		case '?':
		default:
			printf ("Commands:\n");
			printf ("i N ... insert N into tree\n");
			printf ("d N ... delete N from tree\n");
			printf ("f N ... search for N in tree\n");
			printf ("s   ... display tree if not big\n");
			printf ("v   ... print values in tree\n");
			printf ("t   ... run tests on tree\n");
			printf ("q   ... quit\n");
			show = false;
			break;
		}

		if (line[0] == 'q') break;

		if (show) {
			printf ("Tree:\n");
			showTree (mytree);
		}
		printf ("\n> ");
	}

	dropTree (mytree);
	free (values);

	return EXIT_SUCCESS;
}

// run search tests on tree
static void runTests (Tree t, int *values, int N, char order)
{
	Key not[RANGE];

	printf ("Tree search test\n");

	{
		// search for values known to be in tree
		srand (0);		 // for consistency
		int x = rand () % N; // in case the first is reuse
		int ok = 0;
		for (int i = 0; i < N; i++) {
			if (rand () % 10 < 1) // on 1 in 10 loops
				x = x + 1 - 1;	// reuse last search key
			else
				x = rand () % N; // random search key
			// should be found
			if (TreeFind (t, values[x]))
				ok++;
		}
		printf ("Search for %d values known to be in tree\n", N);
		printf ("Found %d matches; ", ok);
		printf ("%s\n", (ok == N) ? "ok" : "not ok");
	}

	// set up array of values *not* in tree
	{
		int NN = 0;
		not[NN++] = 0;
		not[NN++] = RANGE;

		int start, incr;
		switch (order) {
		case 'A': start = N; incr = 1; break;
		case 'D': start = values[N - 1] - 1; incr = -1; break;
		case 'P': start = N; incr = 1; break;
		case 'R': start = 0; incr = 1; break;
		default: usage ();
		}

		int ok = 0;
		for (int x = start; NN < N / 3; x += incr) {
			int i;
			for (i = 0; i < N; i++)
				if (x == values[i])
					break;

			if (i == N) // x is not in tree
				not[NN++] = x;
		}

		// search for values *not* in tree
		for (int i = 0; i < NN; i++)
			if (! TreeFind (t, not[i]))
				ok++;

		printf ("Search for %d values known to *not* be in tree\n", NN);
		printf ("Found %d matches; ", NN - ok);
		printf ("%s\n", (ok == NN) ? "ok" : "not ok");
	}
}

// generate array of values to be inserted in tree
static int *makeValues (int N, char order, int seed)
{
	int *values = malloc ((size_t) N * sizeof (int));
	if (values == NULL) err (EX_OSERR, "couldn't allocate values");

	switch (order) {
	case 'A':
		seed = 1;
		for (int i = 0; i < N; i++) values[i] = seed++;
		break;

	case 'D':
		seed = N;
		for (int i = 0; i < N; i++) values[i] = seed--;
		break;

	case 'P':
		ix = 0;
		mkprefix (values, N, 1, N);
		break;

	case 'R':
		srand ((unsigned) seed);
		mkuniq (values, N);
		break;

	default:
		usage ();
	}

	return values;
}

// create the tree and insert the values
static Tree makeTree (int *values, int N, char style)
{
	Style ins;

	switch (style) {
	case 'L': ins = InsertAtLeaf; break;
	case 'A': ins = InsertAtRoot; break;
	case 'R': ins = InsertRandom; break;
	case 'B': ins = InsertRebalance; break;
	case 'S': ins = InsertSplay; break;
	case 'V': ins = InsertAVL; break;
	default: usage ();
	}

	Tree t = newTree (ins);
	for (int i = 0; i < N; i++)
		TreeInsert (t, values[i]);
	return t;
}

static void mkprefix (int *v, int N, int lo, int hi)
{
	if (ix >= N || lo > hi) return;
	int mid = (lo + hi) / 2;
	v[ix++] = mid;
	mkprefix (v, N, lo, mid - 1);
	mkprefix (v, N, mid + 1, hi);
}

static void mkuniq (int *v, int N)
{
	bool already[RANGE] = {false};
	for (int i = 0; i < N; i++) {
		int x = 1 + rand () % (RANGE - 1);
		if (already[x]) { i--; continue; }
		already[x] = true;
		v[i] = x;
	}
}

static void usage (void)
{
	// on Linux, libbsd provides `getprogname'.
	const char *getprogname (void);

	fprintf (
		stderr,
		"Usage: %s N Order Insert Seed\n"
		"0 <= N <= 9999, Seed = a random number\n"
		"Order = Ascending|Descending|Prefix|Random\n"
		"Insert = Leaf|At-root|reBalance|Random|Splay|aVl\n"
		"For Order and Insert, use just the upper-case letter\n"
		"e.g. for AVL, use V; for Rebalancing, use B\n",
		getprogname ()
	);
	exit (EX_USAGE);
}
