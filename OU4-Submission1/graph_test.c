#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "dlist.h"
#include "graph.h"


void empty_graph_is_empty(void)
{
	graph *g = graph_empty(1);
	assert(graph_is_empty(g) == true);
	graph_kill(g);
}

void nonempty_graph_is_not_empty(void)
{
 	graph *g = graph_empty(1);
 	g = graph_insert_node(g, "AAA");
 	assert(graph_is_empty(g) == false);
 	graph_kill(g);
}

void nonexistent_node_is_not_found(void)
{
	graph *g = graph_empty(1);
	g = graph_insert_node(g, "AAA");
	assert(graph_find_node(g, "ZZZ") == NULL);
	graph_kill(g);
}

void existent_node_is_found(void)
{
	graph *g = graph_empty(1);
	g = graph_insert_node(g, "AAA");
	node *n = graph_find_node(g, "AAA");
	
	assert(nodes_are_equal(n, graph_find_node(g, "AAA")) == true);

	graph_kill(g);
}

void node_is_equal_itself(void)
{
	graph *g = graph_empty(1);
	g = graph_insert_node(g, "AAA");
	node *n = graph_find_node(g, "AAA");

	assert(nodes_are_equal(n, n) == true);

	graph_kill(g);
}

void node_is_not_equal_other(void)
{
	graph *g = graph_empty(2);
	g = graph_insert_node(g, "AAA");
	g = graph_insert_node(g, "BBB");
	node *n1 = graph_find_node(g, "AAA");
	node *n2 = graph_find_node(g, "BBB");

	assert(nodes_are_equal(n1, n2) == false);

	graph_kill(g);
}

void seen_node_is_seen(void)
{
	graph *g = graph_empty(1);
	g = graph_insert_node(g, "AAA");
	node *n = graph_find_node(g, "AAA");
	g = graph_node_set_seen(g, n, true);

	assert(graph_node_is_seen(g, n) == true);

	graph_kill(g);
}

void unseen_node_is_unseen(void)
{
	graph *g = graph_empty(1);
	g = graph_insert_node(g, "AAA");
	node *n = graph_find_node(g, "AAA");
	g = graph_node_set_seen(g, n, false);

	assert(graph_node_is_seen(g, n) == false);

	graph_kill(g);
}

void reset_graph_has_unseen_node(void)
{
	graph *g = graph_empty(1);
	g = graph_insert_node(g, "AAA");
	node *n = graph_find_node(g, "AAA");
	g = graph_node_set_seen(g, n, true);
	g = graph_reset_seen(g);
	assert(graph_node_is_seen(g, n) == false);
	graph_kill(g);
}

void node_without_neighbors_has_none(void)
{
	graph *g = graph_empty(2);
	g = graph_insert_node(g, "AAA");
	node *n = graph_find_node(g, "AAA");
	dlist *neighbours = graph_neighbours(g, n);

	assert(dlist_is_empty(neighbours) == true);

	dlist_kill(neighbours);
	graph_kill(g);
}

void node_with_neighbour_has_same(void)
{
	graph *g = graph_empty(2);
	g = graph_insert_node(g, "AAA");
	g = graph_insert_node(g, "BBB");
	node *n1 = graph_find_node(g, "AAA");
	node *n2 = graph_find_node(g, "BBB");	
	g = graph_insert_edge(g, n1, n2);
	dlist *neighbours = graph_neighbours(g, n1);
	assert(dlist_is_empty(neighbours) == false);
	node *neighbour = dlist_inspect(neighbours, dlist_first(neighbours));
	assert(nodes_are_equal(n2, neighbour) == true);
	dlist_kill(neighbours);
	graph_kill(g);
}

void node_with_three_neighbours_has_same(void)
{
	graph *g = graph_empty(4);
	g = graph_insert_node(g, "AAA");
	g = graph_insert_node(g, "BBB");
	g = graph_insert_node(g, "CCC");
	g = graph_insert_node(g, "DDD");
	node *n1 = graph_find_node(g, "AAA");
	node *n2 = graph_find_node(g, "BBB");
	node *n3 = graph_find_node(g, "CCC");
	node *n4 = graph_find_node(g, "DDD");
	g = graph_insert_edge(g, n1, n2);
	g = graph_insert_edge(g, n1, n3);
	g = graph_insert_edge(g, n1, n4);

	dlist *neighbours = graph_neighbours(g, n1);

	assert(dlist_is_empty(neighbours) == false);

	dlist *known_neighbours = dlist_empty(NULL);
	dlist_insert(known_neighbours, n2, dlist_first(known_neighbours));
	dlist_insert(known_neighbours, n3, dlist_first(known_neighbours));
	dlist_insert(known_neighbours, n4, dlist_first(known_neighbours));

	int neighbour_count = 0;
	while (!dlist_is_empty(neighbours)) {
		node *returned_node = dlist_inspect(neighbours, dlist_first(neighbours));
		dlist_pos pos = dlist_first(known_neighbours);
		while (!dlist_is_end(known_neighbours, pos)) {
			node *known_node = dlist_inspect(known_neighbours, pos);
			if (nodes_are_equal(known_node, returned_node)) {
				neighbour_count++;
				break;
			}
			pos = dlist_next(known_neighbours, pos);
		}
		dlist_remove(neighbours, dlist_first(neighbours));
	}
	assert(neighbour_count == 3);

	dlist_kill(known_neighbours);
	dlist_kill(neighbours);
	graph_kill(g);
}

void test(char *name, void (*t)(void))
{
	fprintf(stderr, "Running test:\t%s:\t", name);
	t();
	fprintf(stderr, "OK.\n");
}

void run_tests(void) {
	test("empty_graph_is_empty", empty_graph_is_empty);
	test("nonempty_graph_is_not_empty", nonempty_graph_is_not_empty);
	test("nonexistent_node_is_not_found", nonexistent_node_is_not_found);
	test("existent_node_is_found", existent_node_is_found);
	test("node_is_equal_itself", node_is_equal_itself);
	test("node_is_not_equal_other", node_is_not_equal_other);
	test("seen_node_is_seen", seen_node_is_seen);
	test("unseen_node_is_unseen", unseen_node_is_unseen);
	test("reset_graph_has_unseen_node", reset_graph_has_unseen_node);
	test("node_without_neighbors_has_none", node_without_neighbors_has_none);
	test("node_with_neighbour_has_neighbour", node_with_neighbour_has_same);
	test("node_with_three_neighbours_has_same", node_with_three_neighbours_has_same);
	printf("FLAWLESS \n");
}

int main(int argc, char const *argv[])
{
	run_tests();
	return EXIT_SUCCESS;
}
