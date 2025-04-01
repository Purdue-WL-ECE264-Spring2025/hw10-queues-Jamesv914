#include "queue.h"
#include "tile_game.h"
#include <stdlib.h>
#include <stdbool.h>

void enqueue(struct queue *q, struct game_state state) {
    size_t value = serialize(state);
    insert_at_tail(&q->data, value); // FIFO: enqueue at tail
}

struct game_state dequeue(struct queue *q) {
    size_t value = remove_from_head(&q->data); // FIFO: dequeue from head
    return deserialize(value);
}

int number_of_moves(struct game_state start) {
    struct queue q = {0};
    struct linked_list visited = {0};

    enqueue(&q, start);
    insert_at_tail(&visited, serialize(start));

    while (q.data.head != NULL) {
        struct game_state cur = dequeue(&q);

        if (is_solved(cur)) {
            free_list(q.data);
            free_list(visited);
            return cur.num_steps;
        }

        for (int dir = 0; dir < 4; dir++) {
            struct game_state next = cur;
            if (make_move(&next, dir)) {
                size_t hash = serialize(next);

                // Linear search to check if state was already visited
                bool seen = false;
                for (struct list_node *n = visited.head; n != NULL; n = n->next) {
                    if (n->value == hash) {
                        seen = true;
                        break;
                    }
                }

                if (!seen) {
                    next.num_steps = cur.num_steps + 1;
                    insert_at_tail(&visited, hash);
                    enqueue(&q, next);
                }
            }
        }
    }

    free_list(q.data);
    free_list(visited);
    return -1;  // Unreachable state
}