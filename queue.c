#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
struct list_head *q_new()
{
    element_t *new = malloc(1 * sizeof(element_t));
    if (!new)
        return NULL;
    INIT_LIST_HEAD(&new->list);
    new->value = NULL;
    return &new->list;
}

/* Free all storage used by queue */
void q_free(struct list_head *l)
{
    element_t *ele;
    if (!l)
        return;
    struct list_head *next = l->next;

    if (next != l)
        do {
            ele = list_entry(next, element_t, list);
            next = ele->list.next;
            free(ele->value);
            free(ele);
        } while (next != l);
    ele = list_entry(l, element_t, list);
    free(ele);
    return;
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(struct list_head *head, char *s)
{
    char *buf;
    unsigned long len = strlen(s);
    element_t *ele_p;

    if (!head)
        return false;
    struct list_head *tmp;  // = q_new();
    if (!(tmp = q_new()) || !(buf = (char *) malloc(1 + len))) {
        if (tmp)
            q_free(tmp);
        return false;
    }
    list_add(tmp, head);
    strncpy(buf, s, len);
    buf[len] = '\0';
    ele_p = list_entry(tmp, element_t, list);
    ele_p->value = buf;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(struct list_head *head, char *s)
{
    char *buf;
    unsigned long len = strlen(s);
    element_t *ele_p;

    if (!head)
        return false;
    struct list_head *tmp = q_new();
    if (!tmp)
        return false;
    buf = (char *) malloc(1 + len);
    if (!buf) {
        q_free(tmp);
        return false;
    }
    list_add_tail(tmp, head);
    strncpy(buf, s, len);
    buf[len] = '\0';
    ele_p = list_entry(tmp, element_t, list);
    ele_p->value = buf;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return target element.
 * Return NULL if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 *
 * NOTE: "remove" is different from "delete"
 * The space used by the list element and the string should not be freed.
 * The only thing "remove" need to do is unlink it.
 *
 * REF:
 * https://english.stackexchange.com/questions/52508/difference-between-delete-and-remove
 */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head)
        return NULL;
    element_t *ret = list_entry(head->next, element_t, list);
    if (!ret->value)
        return NULL;
    struct list_head *targ = head->next;
    if (targ->next != targ->prev) {
        head->next = targ->next;
        targ = targ->next;
        targ->prev = head;
    } else
        head->next = head->prev = head;
    if (sp != NULL) {
        strncpy(sp, ret->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }

    return ret;
}

/*
 * Attempt to remove element from tail of queue.
 * Other attribute is as same as q_remove_head.
 */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head)
        return NULL;
    element_t *ret = list_entry(head->prev, element_t, list);
    if (!ret->value)
        return NULL;
    struct list_head *targ = head->prev;
    if (targ->next != targ->prev) {
        head->prev = targ->prev;
        targ = targ->prev;
        targ->next = head;
    } else
        head->next = head->prev = head;
    if (sp != NULL) {
        strncpy(sp, ret->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }


    return ret;
}

/*
 * WARN: This is for external usage, don't modify it
 * Attempt to release element.
 */
void q_release_element(element_t *e)
{
    free(e->value);
    free(e);
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;

    int len = 0;
    struct list_head *li;

    list_for_each (li, head)
        len++;
    return len;
}

/*
 * Delete the middle node in list.
 * The middle node of a linked list of size n is the
 * ⌊n / 2⌋th node from the start using 0-based indexing.
 * If there're six element, the third member should be return.
 * Return true if successful.
 * Return false if list is NULL or empty.
 */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-listb/
    int len = 0;
    int mid = 0;
    struct list_head *target = head;
    element_t *ele;

    len = q_size(head);
    if (!len)
        return false;
    mid = len / 2;
    for (int i = 0; i < mid + 1; i++)
        target = target->next;
    ele = list_entry(target, element_t, list);
    list_del(target);
    q_release_element(ele);
    return true;
}

/*
 * Delete all nodes that have duplicate string,
 * leaving only distinct strings from the original list.
 * Return true if successful.
 * Return false if list is NULL.
 *
 * Note: this function always be called after sorting, in other words,
 * list is guaranteed to be sorted in ascending order.
 */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    return true;
}

/*
 * Attempt to swap every two adjacent nodes.
 */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    struct list_head *node1, *node2, *tmp;

    if (!head || (head->next == head))
        return;
    node1 = head->next;
    node2 = node1->next;
    while ((node1 != head) && (node2 != head)) {
        tmp = node1->prev;
        tmp->next = node2;
        node2->prev = tmp;
        tmp = node2->next;
        node2->next = node1;
        tmp->prev = node1;
        node1->next = tmp;
        node1->prev = node2;
        node1 = node1->next;
        node2 = node1->next;
    }
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(struct list_head *head)
{
    struct list_head *node, *next_node;

    if (!head || (head->next == head))
        return;
    node = head;
    next_node = node->next;
    while (next_node != head) {
        node->next = node->prev;
        node->prev = next_node;
        node = next_node;
        next_node = node->next;
    }
    node->next = node->prev;
    node->prev = next_node;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(struct list_head *head) {}
