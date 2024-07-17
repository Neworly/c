








#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef char* cstring;

typedef struct node_t {
  struct node_t *prev;
  struct node_t *next;
  cstring value;
} Node;

typedef struct {
  struct content_t {
    Node *(*others);
    struct body_t {
      Node *head;
      Node *cursor;
      Node *tail;
    } base;
  } content;

  size_t length;
  size_t unified;
} Metadata;

#define ASSERT(expr, msg) if (expr) (printf(msg), exit(0))

#define DANGER_FILL_DATA "attempt to fill data to `NULL`"
#define DANGER_INDEX_OUT "trying to index a non registered node"
#define DANGER_HEAD_CUT "you cannot unregister the head node"
#define DANGER_CANNOT_UNIFY "impossible to link1's tail with link2's head"

void update_this_node (Node *this, cstring value, struct node_t *prev, struct node_t *next)
{
  this->prev = prev;
  this->next = next;
  this->value = value;
}

void node_fill_data (Node *this, cstring value, struct node_t *prev, struct node_t *next)
{
  ASSERT(this == NULL, DANGER_FILL_DATA);
  update_this_node(this, value, prev, next);
}

Metadata link_sample ()
{
  Node *base = malloc(sizeof(Node));
  node_fill_data(base, NULL, NULL, NULL);

  return (Metadata)
  {
    .content = {
      .base = (struct body_t) {
        .head = base,
        .cursor = NULL,
        .tail = NULL,
      },
      .others = NULL,
    }, .length = 0, .unified = 0,
  };
}

#define nmalloc() malloc(sizeof(Node))

Node *node_newborn (cstring value)
{
  Node *it = nmalloc();
  update_this_node(it, value, NULL, NULL);
  return it;
}

void attach_node (Node *to, Node *it)
{
  Node *prev = to;
  to->next = it;
  it->prev = prev;
}

void push_linklist_node (Metadata *link, Node *it)
{
  size_t length = link->length + 1;
  link->length = length;

  if (NULL == link->content.base.tail) {
    attach_node(link->content.base.head, it);
    link->content.base.tail = link->content.base.head->next;
    return;
  }

  attach_node(link->content.base.tail, it);
  link->content.base.tail = link->content.base.tail->next;
}

void move_cursor_at (Metadata *link, size_t index) {
  ASSERT(index == 0, DANGER_HEAD_CUT);
  ASSERT(index > link->length, DANGER_INDEX_OUT);

  link->content.base.cursor = link->content.base.head;

  size_t aim = 0;
  while (index != (aim++)) {
    link->content.base.cursor = link->content.base.cursor->next;
  }
}

Node *get_node_details (Metadata *link, size_t index)
{
  move_cursor_at(link, index);
  return link->content.base.cursor;
}

void remove_linklist_node (Metadata *link, size_t index)
{
  move_cursor_at(link, index);
  Node *it = link->content.base.cursor;

  if (it->prev != NULL) {
    if (NULL != it->next) {
      link->content.base.cursor->prev->next = it->next;
    } else {
      link->content.base.cursor->prev->next = NULL;
    }
  }

  free(it);
}

void unify (Metadata *link1, Metadata *link2)
{
  ASSERT(NULL == link1->content.base.tail, DANGER_CANNOT_UNIFY);
  attach_node(link1->content.base.tail, link2->content.base.head->next);

  link1->content.others = realloc(link1->content.others, sizeof(Node) * (++link1->unified));
  link1->content.others[link1->unified - 1] = link2->content.base.head;

  link1->length += link2->length;
}

int main (void)
{

  Metadata link1 = link_sample();
  push_linklist_node(&link1, node_newborn("Hello"));
  push_linklist_node(&link1, node_newborn("Hello 1"));
  push_linklist_node(&link1, node_newborn("Hello 2"));

  Metadata link2 = link_sample();
  push_linklist_node(&link2, node_newborn("Hello 3"));
  push_linklist_node(&link2, node_newborn("Hello 4"));
  push_linklist_node(&link2, node_newborn("Hello 5"));

  unify(&link1, &link2);
  printf("%s", get_node_details(&link1, 6)->value);

  return 0;
}
