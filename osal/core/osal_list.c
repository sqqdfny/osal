/**
 * @file osal_lsit.c
 * @brief list
 * @date 2022-01-21
 * @author sqqdfny
 *         sqqdfny@163.com
 */

#include "osal_typedef.h"
#include "osal_list.h"

#define LIST_POISON1  NULL
#define LIST_POISON2  NULL
//==================================================================================================
void INIT_LIST_HEAD(struct list_head *list)
{
    list->next = list;
    list->prev = list;
}

static void __list_add(struct list_head *new, struct list_head *prev, struct list_head *next)
{
    next->prev = new;
    new->next = next;
    new->prev = prev;
    prev->next = new;
}

void list_add(struct list_head *new, struct list_head *head)
{
    __list_add(new, head, head->next);
}

void list_add_tail(struct list_head *new, struct list_head *head)
{
    __list_add(new, head->prev, head);
}

static void __list_del(struct list_head *prev, struct list_head *next)
{
    next->prev = prev;
    prev->next = next;
}

void list_del(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
    entry->next = LIST_POISON1;
    entry->prev = LIST_POISON2;
}

void list_del_init(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
    INIT_LIST_HEAD(entry);
}

void list_replace(struct list_head *old, struct list_head *new)
{
    new->next = old->next;
    new->next->prev = new;
    new->prev = old->prev;
    new->prev->next = new;
}

void list_replace_init(struct list_head *old, struct list_head *new)
{
    list_replace(old, new);
    INIT_LIST_HEAD(old);
}

int list_empty(const struct list_head *head)
{
    return head->next == head ? 1 : 0;
}
//==================================================================================================
//end files

