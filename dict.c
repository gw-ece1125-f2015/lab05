#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_KEY_LEN (32)
#define MAX_VALUE_LEN (32)
struct list_data {
	char key[MAX_KEY_LEN];
	char value[MAX_VALUE_LEN];
};

struct list_node {
	struct list_data data;
	struct list_node *next;
	struct list_node *prev;
};

void list_print(struct list_node *head)
{
}

struct list_node *list_find_exact(struct list_node *head, const char *key)
{
}

struct list_node *list_find_before(struct list_node *head, const char *key)
{
}

int list_insert(struct list_node **head, struct list_node *before, struct list_data data)
{
}

/* Here we can directly pass the node to be removed. */
void list_remove(struct list_node **p_head, struct list_node *remove_node, struct list_data *p_data)
{
}



void list_destroy(struct list_node *head)
{
}



int main(int argc, char *argv[])
{
	int use_sorted = 0;

	if (argc < 3) {
		fprintf(stderr, "Usage: dict sort data_file.txt [lookup_commands.txt] [remove_commands.txt]\n"
			         "sort: 0 or 1\n");
		return 1;
	}

	use_sorted = strtol(argv[1], NULL, 10);

	FILE *fp_data = fopen(argv[2], "rb");
	if (fp_data == NULL) {
		fprintf(stderr, "Error opening %s\n", argv[1]);
		return 1;
	}

	FILE *fp_lookup = NULL;
	if (argc > 3) {
		fp_lookup = fopen(argv[3], "rb");
		if (fp_lookup == NULL) {
			fprintf(stderr, "Error opening lookups file %s\n", argv[3]);
			return 1;
		}
	}

	FILE *fp_removal = NULL;
	if (argc > 4) {
		fp_removal = fopen(argv[4], "rb");
		if (fp_removal == NULL) {
			fprintf(stderr, "Error opening removals file %s\n", argv[4]);
			return 1;
		}
	}

	struct list_data d;
	struct list_node *head = NULL;
	
	while (!feof(fp_data) && !ferror(fp_data)) {
		if (fscanf(fp_data, "%s %s", d.key, d.value) != 2)
			continue;
		
		if (use_sorted) {
			printf("Insert %s %s in sorted order\n", d.key, d.value);
			struct list_node *node_before = list_find_before(head, d.key);
			if (list_insert(&head, node_before, d) < 0) {
				fprintf(stderr, "Error while inserting into list: %s\n", strerror(errno));
			}
		}
		else {
			printf("Insert %s %s in front\n", d.key, d.value);
			if (list_insert(&head, NULL, d) < 0) {
				fprintf(stderr, "Error while inserting into list: %s\n", strerror(errno));
			}
		}

		list_print(head);
	}


	char lookup_key[MAX_KEY_LEN];
	struct list_node *lookup_node;

	while (fp_lookup && !feof(fp_lookup) && !ferror(fp_lookup)) {
		if (fscanf(fp_lookup, "%s", lookup_key) != 1)
			continue;

		if ((lookup_node = list_find_exact(head, lookup_key)) != NULL) {
			printf("Found: %s %s\n", lookup_node->data.key, lookup_node->data.value);
		}
		else {
			printf("Failed to find: %s\n", lookup_key);
		}
	}

	char removal_key[MAX_KEY_LEN];
	struct list_node *removal_node;

	if (fp_removal) {
		printf("Now removing elements.\n");
	}

	while (fp_removal && !feof(fp_removal) && !ferror(fp_removal)) {
		if (fscanf(fp_removal, "%s", removal_key) != 1)
			continue;

		if ((removal_node = list_find_exact(head, removal_key)) != NULL) {
			struct list_data removed_data;
			printf("Found node to remove: %s %s\n", removal_node->data.key, removal_node->data.value);
			list_remove(&head, removal_node, &removed_data);
			printf("Removed node: %s %s\n", removed_data.key, removed_data.value);
		}
		else {
			printf("Failed to find node to remove: %s\n", removal_key);
		}

		list_print(head);
	}

	list_destroy(head);
	
	fclose(fp_data);

	if (fp_lookup) {
		fclose(fp_lookup);
	}

	if (fp_removal) {
		fclose(fp_removal);
	}

	
	return 0;
}
