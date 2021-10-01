#include "ll_cycle.h"

bool ll_has_cycle(node *head)
{
    if (!head)
        return false;
    node *next = head->next;
    node *previous = head;
    while (next)
    {
        if (next == previous) 
            return true;
        previous = previous->next;
        next = next->next;
        if (next == nullptr)
            return false;
        next = next->next;
    }
    return false;
}
