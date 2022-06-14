int get_size(ListNode *&head_node)
{
    int count_list_size = 0;
    if (head_node != NULL)
    {
        for (ListNode *p = head_node; p != NULL; p = p->next)
        {
            count_list_size++;
        }
        return count_list_size;
    }
    else
    {
        return count_list_size;
    }
}

ListNode *list_create(void)
{
    ListNode *head = new ListNode;
    head->index = 0;
    head->next = NULL;
    return head;
}

bool node_delete(ListNode *&head_node, int n)
{
    if (n >= 0 && n < get_size(head_node))
    {
        if (n != 0)
        {
            int i = 0;
            for (ListNode *p = head_node; p != NULL; p = p->next)
            {
                if (i == n - 1)
                {
                    p->next = p->next->next;
                    return true;
                }
                i++;
            }
            cout << "\n[Error] Node doesn't exist" << '\n';
            return false;
        }
        else
        {
            head_node = head_node->next;
            return true;
        }
        return false;
    }
    else
    {
        cout << "\n[Error] Size overflow in: node_delete(ListNode *&head_node, int n)" << '\n';
        cout << "                                                            ~~~~^ n = " << n << '\n';
        return false;
    }
}

void list_delete(ListNode *&head_node)
{
    for (int i = get_size(head_node) - 1; i >= 0; --i)
    {
        node_delete(head_node, i);
    }
}

void list_printf(ListNode *head_node)
{
    for (ListNode *p = head_node; p != NULL; p = p->next)
    {
        cout << "node[" << p->index << "]: " << p << ' ' << p->data << '\n';
    }
}