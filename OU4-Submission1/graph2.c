// Standard libraries 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Interface declarations
#include "graph.h"
#include "array_1d.h"
#include "array_2d.h"
#include "dlist.h"

/*
* Implementation of graph utilizing 2D array/adjacency matrix for the 
* "Datastructures and algorithms" courses at the Department of Computing Science, 
* Umea University.
*
* Authors: Jesper Erixon        (tfy17jen@cs.umu.se)
*          Henrik Linder        (tfy18hlr@cs.umu.se)
*          Lisa Olofsson        (ens18lon@cs.umu.se)
*
* Version information:
*   2023-05-19: v1.0, first submission.
*/

/*Defines a graph structure */
struct graph
{
        int node_max;           // Max allowed number of nodes, 
                                // needed for static arrays
        int nodes_inserted;     // Number of nodes inserted
        array_1d *nodes;        // 1D array for node names
        array_2d *adj;          // 2D adjacency matrix
};

/*Defines a structure for the nodes */
struct node
{
        const char *identifier; // i.e. Name
        int index;              // To corresponding adjacency matrix
        bool seen_status;       // Seen status
};

// =================== NODE COMPARISON FUNCTION ======================

/**
 * nodes_are_equal() - Check whether two nodes are equal.
 * @n1: Pointer to node 1.
 * @n2: Pointer to node 2.
 *
 * Returns: true if the nodes are considered equal, otherwise false.
 *
 */
bool nodes_are_equal(const node *n1,const node *n2)
{
        const char *s1 = n1->identifier;
        const char *s2 = n2->identifier;
        /*strcmp returns 0 if strings are equal hence !strcmp*/
        return !strcmp(s2, s1);
}

// =================== GRAPH STRUCTURE INTERFACE ======================

/**
 * graph_empty() - Create an empty graph.
 * @max_nodes: The maximum number of nodes the graph can hold.
 *
 * Returns: A pointer to the new graph.
 */
graph *graph_empty(int max_nodes)
{
        // Allocates memory for graph
        graph *g = malloc(sizeof(graph));
        // Create empty 1D array for nodes
        g->nodes = array_1d_create(0,max_nodes-1,NULL);
        // Create empty 2D array / adjacency matrix, i.e. no connections
        g->adj = array_2d_create(0,max_nodes-1,0,max_nodes-1,free);
        // Maximum allowed number of nodes
        g->node_max = max_nodes;
        // Set number of inserted nodes to zero  
        g->nodes_inserted = 0;
        return g;
}

/**
 * graph_is_empty() - Check if a graph is empty, i.e. has no nodes.
 * @g: Graph to check.
 *
 * Returns: True if graph is empty, otherwise false.
 */
bool graph_is_empty(const graph *g)
{       
        return (g->nodes_inserted == 0);
}

/**
 * graph_has_edges() - Check if a graph has any edges.
 * @g: Graph to check.
 *
 * Returns: True if graph has any edges, otherwise false.
 */
bool graph_has_edges(const graph *g)
{
        // Loop over adjacency matrix, check if empty  
        for (int i = 0; i < g->node_max; i++)
        {
                for (int j = 0; i < g->node_max; j++)
                {
                        if (array_2d_has_value(g->adj,i,j))
                        {
                                // Return true if there is a edge between any nodes 
                                return true;
                        }           
                }
        }
        return false;
}

/**
 * graph_insert_node() - Inserts a node with the given name into the graph.
 * @g: Graph to manipulate.
 * @s: Node name.
 *
 * Creates a new node with a copy of the given name and puts it into
 * the graph.
 *
 * Returns: The modified graph.
 */
graph *graph_insert_node(graph *g, const char *s)
{       
        // Check if max number of nodes exceeded
        if (g->nodes_inserted >= g->node_max)
        {
                printf("ERROR: Maximum number of nodes exceeded!\n");
                return g; 
        }

        // Allocates memory for node
        node *n = malloc(sizeof(node));
        // Set name
        n->identifier = s;
        // Give index
        n->index = g->nodes_inserted; 
        // Not seen
        n->seen_status = false;
        // Insert value into node array
        array_1d_set_value(g->nodes, n, n->index);
        // Count up number of inserted nodes in graph
        g->nodes_inserted++;
        // Return graph with inserted node
        return g;
}

/**
 * graph_find_node() - Find a node stored in the graph.
 * @g: Graph to manipulate.
 * @s: Node identifier, e.g. a char *.
 *
 * Returns: A pointer to the found node, or NULL.
 */
node *graph_find_node(const graph *g, const char *s)
{       
        // Loop over inserted number of nodes
        for (int i = 0; i < g->nodes_inserted; i++)
        {       // Inspect node
                node *inspected_node = array_1d_inspect_value(g->nodes,i);
                // If the inspected node's name matches input string...
                if (!strcmp(inspected_node->identifier, s))
                {
                        //...return that node
                        return inspected_node;
                }
        }
        //If node is not found, return NULL       
        return NULL;
}

/**
 * graph_node_is_seen() - Return the seen status for a node.
 * @g: Graph storing the node.
 * @n: Node in the graph to return seen status for.
 *
 * Returns: The seen status for the node.
 */
bool graph_node_is_seen(const graph *g, const node *n)
{
        return n->seen_status;
}

/**
 * graph_node_set_seen() - Set the seen status for a node.
 * @g: Graph storing the node.
 * @n: Node in the graph to set seen status for.
 * @s: Status to set.
 *
 * Returns: The modified graph.
 */
graph *graph_node_set_seen(graph *g, node *n, bool seen)
{        
        n->seen_status = seen;
        return g;
}

/**
 * graph_reset_seen() - Reset the seen status on all nodes in the graph.
 * @g: Graph to modify.
 *
 * Returns: The modified graph.
 */
graph *graph_reset_seen(graph *g)
{       
        // Loop over all inserted nodes
        for (int i = 0; i < g->nodes_inserted; i++)
        {       
                // Inspect node with index i
                node *inspected_node = array_1d_inspect_value(g->nodes,i);
                // Reset seen status, i.e set to false
                inspected_node->seen_status = false;
        }
        return g;
}

/**
 * graph_insert_edge() - Insert an edge into the graph.
 * @g: Graph to manipulate.
 * @n1: Source node (pointer) for the edge.
 * @n2: Destination node (pointer) for the edge.
 *
 * NOTE: Undefined unless both nodes are already in the graph.
 *
 * Returns: The modified graph.
 */
graph *graph_insert_edge(graph *g, node *n1, node *n2)
{
        // Allocate memory for (int) value 
        int *v=malloc(sizeof(int));
        // Set value to 1 to indicate that n2 is a neighbour to n1
        *v = 1; 
        // Insert value into adjacency matrix
        array_2d_set_value(g->adj,v,n1->index,n2->index); 
        return g;
}

/** <NOT IMPLEMENTED / USED>
 * graph_delete_node() - Remove a node from the graph.
 * @g: Graph to manipulate.
 * @n: Node to remove from the graph.
 *
 * Returns: The modified graph.
 *
 * NOTE: Undefined if the node is not in the graph.
 */
// graph *graph_delete_node(graph *g, node *n);

/** <NOT IMPLEMENTED / USED>
 * graph_delete_edge() - Remove an edge from the graph.
 * @g: Graph to manipulate.
 * @n1: Source node (pointer) for the edge.
 * @n2: Destination node (pointer) for the edge.
 *
 * Returns: The modified graph.
 *
 * NOTE: Undefined if the edge is not in the graph.
 */

// graph *graph_delete_edge(graph *g, node *n1, node *n2);

/** <NOT IMPLEMENTED / USED>
 * graph_choose_node() - Return an arbitrary node from the graph.
 * @g: Graph to inspect.
 *
 * Returns: A pointer to an arbitrary node.
 *
 * NOTE: The return value is undefined for an empty graph.
 */
// node *graph_choose_node(const graph *g)
// {       
//         // Choose first node 
//         int i = 0; 
//         node *inspected_node = array_1d_inspect_value(g->nodes,i);
//         return inspected_node;
//         return inspected_node;
// }

/**
 * graph_neighbours() - Return a list of neighbour nodes.
 * @g: Graph to inspect.
 * @n: Node to get neighbours for.
 *
 * Returns: A pointer to a list of nodes. Note: The list must be
 * dlist_kill()-ed after use.
 */
dlist *graph_neighbours(const graph *g,const node *n) 
{
        // Creates an empty directed list
        dlist *neighbours = dlist_empty(NULL);
        dlist_pos pos = dlist_first(neighbours);

        // If node not found, return empty list        
        if(graph_find_node(g, n->identifier)==NULL)
        {
                return neighbours;
        }
        
        // Scan columns of adjacency matrix for connections to other nodes
        for (int j = 0; j < g->node_max; j++)
        {
                // If connection found to node with index i...
                if (array_2d_has_value(g->adj,n->index,j))
                {       // ... create copy of neighboring node and add it to list
                        node *n_adj = array_1d_inspect_value(g->nodes,j); 
                        pos = dlist_insert(neighbours, n_adj, pos);
                }
        }
        return neighbours; 
}

/**
 * graph_kill() - Destroy a given graph.
 * @g: Graph to destroy.
 *
 * Return all dynamic memory used by the graph.
 *
 * Returns: Nothing.
 */
void graph_kill(graph *g)
{       
        // Loop through 1D array and clear/free all nodes
        for (int i = 0; i < g->node_max; i++)
        {
                node *node = array_1d_inspect_value(g->nodes, i);
                free(node);
        }
        // Free up graph strcture elements
        array_2d_kill(g->adj);
        array_1d_kill(g->nodes);
        free(g);
}

/** <NOT IMPLEMENTED / USED>
 * graph_print() - Iterate over the graph elements and print their values.
 * @g: Graph to inspect.
 *
 * Iterates over the graph and prints its contents.
 *
 * Returns: Nothing.
 */
// void graph_print(const graph *g);

