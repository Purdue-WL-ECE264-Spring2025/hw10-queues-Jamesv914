#include "queue.h"
#include "tile_game.h"
#include <stdlib.h>
#include <stdbool.h>

// Forward declarations
bool is_solved(struct game_state state);
bool make_move(struct game_state *state, int direction);

void enqueue(struct queue *q, struct game_state state) {
    size_t value = serialize(state);
    insert_at_tail(&q->data, value);
}

struct game_state dequeue(struct queue *q) {
    size_t value = remove_from_head(&q->data);
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
    return -1;
}

bool is_solved(struct game_state state) {
    uint8_t expected = 1;
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (row == 3 && col == 3) {
                if (state.tiles[row][col] != 0) return false;
            } else {
                if (state.tiles[row][col] != expected) return false;
                expected++;
            }
        }
    }
    return true;
}

bool make_move(struct game_state *state, int direction) {
    switch (direction) {
        case 0: move_up(state); return true;
        case 1: move_down(state); return true;
        case 2: move_left(state); return true;
        case 3: move_right(state); return true;
        default: return false;
    }
}