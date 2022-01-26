


#ifndef __OSALL_LIST_H__
#define __OSALL_LIST_H__


#ifdef __cplusplus
extern "C"
{
#endif

#ifndef container_of
    #define container_of(ptr, type, member)    ((type *)(((const char *)(ptr)) - offsetof(type, member)))
#endif
//==================================================================================================
struct list_head
{
    struct list_head *next, *prev;
}list_head_t;

#define LIST_HEAD_INIT(name) { &(name), &(name) }
#define LIST_HEAD(name) struct list_head name = LIST_HEAD_INIT(name)
void INIT_LIST_HEAD(struct list_head *list);

void list_add(struct list_head *new, struct list_head *head);
void list_add_tail(struct list_head *new, struct list_head *head);

void list_del(struct list_head *entry);
void list_del_init(struct list_head *entry);

void list_replace(struct list_head *old, struct list_head *new);
void list_replace_init(struct list_head *old, struct list_head *new);

int list_empty(const struct list_head *head);

#define list_entry(ptr, type, member)       container_of(ptr, type, member)
#define list_first_entry(ptr, type, member) list_entry((ptr)->next, type, member)
#define list_for_each(pos, head)            for(pos = (head)->next; pos != (head); pos = (pos)->next)
#define list_for_each_safe(pos, n, head)    for(pos = (head)->next, n = (pos)->next; pos != (head); pos = (n), n = (pos)->next)
//==================================================================================================
#ifdef __cplusplus
}
#endif
#endif //__OSALL_LIST_H__
//==================================================================================================
//end files
