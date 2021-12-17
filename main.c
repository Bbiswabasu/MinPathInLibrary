#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
	int first, second;
} Pair;

typedef struct
{
	Pair *data;
	int size, capacity;
} Heap;

void init(Heap *heap, int capacity)
{
	heap->capacity = capacity;
	heap->data = (Pair *)malloc(capacity * sizeof(Pair));
	for (int i = 1; i < capacity; i++)
	{
		(heap->data)[i].first = (1e8);
		(heap->data)[i].second = 0;
	}
	heap->size = 0;
}

void update_up(Heap *heap, int pos)
{
	if (pos == 1)
		return;
	if ((heap->data)[pos / 2].first > (heap->data)[pos].first)
	{
		Pair t = (heap->data)[pos];
		(heap->data)[pos] = (heap->data)[pos / 2];
		(heap->data)[pos / 2] = t;
		update_up(heap, pos / 2);
	}
}
void insert(Heap *heap, int u, int w)
{
	++(heap->size);
	if (heap->size > heap->capacity)
	{
		heap->capacity = (heap->capacity) * 2;
		heap->data = (Pair *)realloc(heap->data, heap->capacity * sizeof(Pair));
	}
	(heap->data)[heap->size].first = w;
	(heap->data)[heap->size].second = u;
	update_up(heap, heap->size);
}

void update_down(Heap *heap, int pos)
{
	if (2 * pos > (heap->size) || ((heap->data)[pos].first < (heap->data)[2 * pos].first && (heap->data)[pos].first < (heap->data)[2 * pos + 1].first))
		return;
	if ((heap->data)[2 * pos].first < (heap->data)[2 * pos + 1].first)
	{
		Pair t = (heap->data)[2 * pos];
		(heap->data)[2 * pos] = (heap->data)[pos];
		(heap->data)[pos] = t;
		update_down(heap, 2 * pos);
	}
	else
	{
		Pair t = (heap->data)[2 * pos + 1];
		(heap->data)[2 * pos + 1] = (heap->data)[pos];
		(heap->data)[pos] = t;
		update_down(heap, 2 * pos + 1);
	}
}
Pair remove_min(Heap *heap)
{
	Pair x = (heap->data)[1];
	(heap->data)[1] = (heap->data)[heap->size];
	(heap->size)--;
	update_down(heap, 1);
	return x;
}

struct AdjListNode
{
	int dest;
	int weight;
	struct AdjListNode *next;
};

struct AdjList
{
	struct AdjListNode *head;
};

struct Graph
{
	int V;
	struct AdjList *array;
};

struct AdjListNode *newAdjListNode(int dest, int weight)
{
	struct AdjListNode *newNode = (struct AdjListNode *)malloc(sizeof(struct AdjListNode));
	newNode->dest = dest;
	newNode->weight = weight;
	newNode->next = NULL;
	return newNode;
}

struct Graph *createGraph(int V)
{
	struct Graph *graph = (struct Graph *)malloc(sizeof(struct Graph));
	graph->V = V;

	graph->array = (struct AdjList *)malloc(V * sizeof(struct AdjList));

	for (int i = 0; i < V; ++i)
		graph->array[i].head = NULL;

	return graph;
}

void addEdge(struct Graph *graph, int src, int dest, int weight)
{

	struct AdjListNode *newNode = newAdjListNode(dest, weight);
	newNode->next = graph->array[src].head;
	graph->array[src].head = newNode;

	newNode = newAdjListNode(src, weight);
	newNode->next = graph->array[dest].head;
	graph->array[dest].head = newNode;
}

void dijkstra(struct Graph *graph, int src, int *dist, int *par)
{
	int V = graph->V;
	for (int i = 0; i < V; i++)
		dist[i] = (int)(1e8);
	Heap *heap = (Heap *)malloc(sizeof(Heap));
	init(heap, V);
	insert(heap, src, 0);
	par[src] = -1;
	dist[src] = 0;
	while (heap->size > 0)
	{
		Pair top = remove_min(heap);
		int d = top.first, u = top.second;
		if (d != dist[u])
			continue;

		struct AdjListNode *node = graph->array[u].head;
		while (node != NULL)
		{
			int v = node->dest;
			if (dist[u] + (node->weight) < dist[v])
			{
				dist[v] = dist[u] + (node->weight);
				par[v] = u;
				insert(heap, v, dist[v]);
			}
			node = node->next;
		}
	}
}

int search(char *s, char **list, int size)
{
	int l = 0, r = size - 1, mid;
	while (l <= r)
	{
		mid = (l + r) / 2;
		int compare = strcmp(s, list[mid]);
		if (compare == 0)
			return 1;
		if (compare > 0)
			l = mid + 1;
		else
			r = mid - 1;
	}
	return 0;
}

int chooseHall(int V, char *tag, char ***tagList, int *dist, int *size)
{
	int hall = -1, minDist = 1e8;
	for (int i = 0; i < V; i++)
	{
		if (search(tag, tagList[i], size[i]) && dist[i] < minDist)
		{
			minDist = dist[i];
			hall = i;
		}
	}
	return hall;
}

void compute_path(int *par, int V, int hall, int distToHall)
{
	printf("Optimal hall-ID: %d\nDistance to hall: %d\n", hall + 1, distToHall);

	int *path = (int *)malloc(V * sizeof(int));
	int node = hall, i = 0;

	while (node != -1)
	{
		path[i++] = node;
		node = par[node];
	}
	printf("Path to hall: %d", path[--i] + 1);
	while (i > 0)
		printf(" -> %d", path[--i] + 1);
	printf("\n");
}
static int comp(const void *, const void *);
int main()
{
	int n, c;
	printf("Enter the number of halls: ");
	scanf("%d", &n);
	struct Graph *g;
	g = createGraph(n); //creation of graph data-structure for the n halls
	printf("Enter the number of connections among the halls: ");
	scanf("%d", &c);
	int i, j, k;
	printf("\nEnter the 2 hall ID's of a pair of connected halls and the distance between them: (for %d records)\n", c);
	for (i = 0; i < c; i++)
	{
		printf("Enter three space separated integers denoting the pair and the distance: ");
		int u, v, w;

		scanf("%d %d %d", &u, &v, &w);
		u--;
		v--;
		addEdge(g, u, v, w); //joining the specified halls by edges
							 //create the connection and store the distance
	}
	printf("Enter the book tags present in respective halls: (space separated strings of total length not more than for 10 for each book)\n");
	char ***books = (char ***)malloc(n * sizeof(char **));
	for (i = 0; i < n; i++)
	{
		books[i] = (char **)malloc(20 * sizeof(char *));
	}
	int *size = (int *)malloc(n * sizeof(int));
	char str[301];
	gets(str); //false input to initiate gets
	for (i = 0; i < n; i++)
	{
		printf("Hall #%d : ", i + 1);
		gets(str);
		int len = strlen(str), pos = 0;
		int cnt = 0;
		for (j = 0; j <= len; j++)
		{
			char w[21];
			if (j == len || str[j] == ' ')
			{
				w[pos] = '\0';
				pos = 0;
				books[i][cnt] = (char *)malloc(10 * sizeof(char));
				strcpy(books[i][cnt], w);
				cnt++;
			}
			else
				w[pos++] = str[j];
		}
		size[i] = cnt;
		qsort(books[i], cnt, sizeof(const char *), comp);
	}
	printf("Enter the Hall ID of the hall the person is currently in (an integer from 1 to n): ");
	int src;
	scanf("%d", &src);
	src--;
	char searchstr[21];
	printf("Enter the book-tag he is searching for: ");
	scanf("%s", searchstr);
	int *dist = (int *)malloc(n * sizeof(int)), *par = (int *)malloc(n * sizeof(int));
	dijkstra(g, src, dist, par);
	int hall = chooseHall(n, searchstr, books, dist, size);
	if (hall == -1)
		printf("Tag not found\n");
	else
		compute_path(par, n, hall, dist[hall]);
}
static int comp(const void *a, const void *b)
{
	return strcmp(*(const char **)a, *(const char **)b) > 0;
}
