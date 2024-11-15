/*
 * =============================================================================
 *
 *       Filename:  rbtree-tst.c
 *
 *    Description:  rbtree testcase.
 *
 *        Created:  09/02/2012 11:39:34 PM
 *
 *         Author:  Fu Haiping (forhappy), haipingf@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdint.h>
#define VALUE_TYPE uint32_t
#define COMPARE_VAL(x, y) ({        \
        x - y;                  \
})
#define NULL_VAL 0
#define TERMINATE_VAL(v) ;

struct mynode {
  	struct rb_node node;
  	VALUE_TYPE value;
};

struct rb_root mytree = RB_ROOT;

struct mynode * my_search(struct rb_root *root, VALUE_TYPE val)
{
  	struct rb_node *node = root->rb_node;

  	while (node) {
  		struct mynode *data = container_of(node, struct mynode, node);
		int result;

        result = COMPARE_VAL(val, data->value);

		if      (result < 0)    node = node->rb_left;
		else if (result > 0)    node = node->rb_right;
		else                    return data;
	}
	return NULL;
}

int my_insert(struct rb_root *root, struct mynode *data)
{
  	struct rb_node **new = &(root->rb_node), *parent = NULL;

  	/* Figure out where to put new node */
  	while (*new) {
  		struct mynode *this = container_of(*new, struct mynode, node);
        int result = COMPARE_VAL(data->value, this->value);

		parent = *new;
  		if (result < 0)
  			new = &((*new)->rb_left);
  		else if (result > 0)
  			new = &((*new)->rb_right);
  		else
  			return 0;
  	}

  	/* Add new node and rebalance tree. */
  	rb_link_node(&data->node, parent, new);
  	rb_insert_color(&data->node, root);

	return 1;
}

void my_free(struct mynode *node)
{
	if (node != NULL) {
		if (node->value != NULL_VAL) {
            TERMINATE_VAL(node->value);
            node->value = NULL_VAL;
		}
		free(node);
		node = NULL;
	}
}

void my_print(struct rb_node *node, int level)
{
    if (!node) {
        for (int i = 0; i < level; i++) printf("|   ");
        printf("NIL\n");
        return;
    }

    struct mynode *data = container_of(node, struct mynode, node);

    for (int i = 0; i < level; i++) printf("|   ");
    printf("%u %lu\n", data->value, rb_color(node));

    my_print(node->rb_left, level + 1);
    my_print(node->rb_right, level + 1);
}

#define NUM_NODES 9

int main()
{

	struct mynode *mn[NUM_NODES];

	/* *insert */
	int i = 1;
	printf("insert node from 1 to NUM_NODES(32): \n");
	for (; i < NUM_NODES; i++) {
		mn[i] = (struct mynode *)malloc(sizeof(struct mynode));
        mn[i]->value = i;
		my_insert(&mytree, mn[i]);
	}
	
	/* *search */
	struct rb_node *node;
	printf("search all nodes: \n");
	for (node = rb_first(&mytree); node; node = rb_next(node))
		printf("key = %u\n", rb_entry(node, struct mynode, node)->value);      // TODO

	/* *delete */
	printf("delete node 20: \n");
	struct mynode *data = my_search(&mytree, 20);     // TODO
	if (data) {
		rb_erase(&data->node, &mytree);
		my_free(data);
	}


	/* *delete once again*/
	printf("delete node 15: \n");
	data = my_search(&mytree, 15);                    // TODO
	if (data) {
		rb_erase(&data->node, &mytree);
		my_free(data);
	}

	/* *search again*/
	printf("search again:\n");
	for (node = rb_first(&mytree); node; node = rb_next(node))
		printf("key = %u\n", rb_entry(node, struct mynode, node)->value);      // TODO

    my_print(mytree.rb_node, 0);

	return 0;
}


