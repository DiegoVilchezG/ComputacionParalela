#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* ── Nodo ── */
typedef struct Node {
    int data;
    struct Node *next;
    pthread_mutex_t lock;   // mutex por nodo
} Node;

/* ── Lista ── */
typedef struct {
    Node *head;
    pthread_mutex_t head_lock;
} LinkedList;

/* ── Inicializar lista ── */
void list_init(LinkedList *list) {
    list->head = NULL;
    pthread_mutex_init(&list->head_lock, NULL);
}

/* ── Crear nodo ── */
Node *create_node(int data) {
    Node *node = malloc(sizeof(Node));
    node->data = data;
    node->next = NULL;
    pthread_mutex_init(&node->lock, NULL);
    return node;
}

/* ── Insertar al frente (thread-safe) ── */
void list_insert_front(LinkedList *list, int data) {
    Node *new_node = create_node(data);

    pthread_mutex_lock(&list->head_lock);
    new_node->next = list->head;
    list->head = new_node;
    pthread_mutex_unlock(&list->head_lock);
}

/* ── Buscar (hand-over-hand) ── */
int list_search(LinkedList *list, int target) {
    pthread_mutex_lock(&list->head_lock);
    Node *curr = list->head;
    if (curr) pthread_mutex_lock(&curr->lock);
    pthread_mutex_unlock(&list->head_lock);

    while (curr != NULL) {
        if (curr->data == target) {
            pthread_mutex_unlock(&curr->lock);
            return 1; // encontrado
        }
        Node *next = curr->next;
        if (next) pthread_mutex_lock(&next->lock);
        pthread_mutex_unlock(&curr->lock);
        curr = next;
    }
    return 0; // no encontrado
}

/* ── Eliminar nodo (hand-over-hand) ── */
void list_delete(LinkedList *list, int target) {
    pthread_mutex_lock(&list->head_lock);
    Node *prev = NULL;
    Node *curr = list->head;

    if (curr) pthread_mutex_lock(&curr->lock);

    while (curr != NULL) {
        if (curr->data == target) {
            if (prev == NULL)
                list->head = curr->next;
            else
                prev->next = curr->next;

            pthread_mutex_unlock(&curr->lock);
            if (prev) pthread_mutex_unlock(&prev->lock);
            else      pthread_mutex_unlock(&list->head_lock);

            pthread_mutex_destroy(&curr->lock);
            free(curr);
            return;
        }
        Node *next = curr->next;
        if (next) pthread_mutex_lock(&next->lock);
        if (prev) pthread_mutex_unlock(&prev->lock);
        else      pthread_mutex_unlock(&list->head_lock);
        prev = curr;
        curr = next;
    }
    if (prev) pthread_mutex_unlock(&prev->lock);
    else      pthread_mutex_unlock(&list->head_lock);
}

/* ── Demo: múltiples hilos buscando en paralelo ── */
typedef struct { LinkedList *list; int target; } SearchArgs;

void *search_thread(void *arg) {
    SearchArgs *a = (SearchArgs *)arg;
    int found = list_search(a->list, a->target);
    printf("Hilo buscando %d: %s\n", a->target, found ? "ENCONTRADO" : "no encontrado");
    return NULL;
}

int main() {
    LinkedList list;
    list_init(&list);

    // Insertar valores
    for (int i = 1; i <= 10; i++)
        list_insert_front(&list, i * 10);

    // Lanzar 4 hilos buscando en paralelo
    pthread_t threads[4];
    SearchArgs args[4] = {
        {&list, 20}, {&list, 50}, {&list, 99}, {&list, 80}
    };

    for (int i = 0; i < 4; i++)
        pthread_create(&threads[i], NULL, search_thread, &args[i]);

    for (int i = 0; i < 4; i++)
        pthread_join(threads[i], NULL);

    return 0;
}
