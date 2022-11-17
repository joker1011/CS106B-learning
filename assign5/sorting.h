#pragma once

#include "listnode.h"

void runSort(ListNode*& front);
void quickSort(ListNode*& front);
ListNode* quickSortRec(ListNode* front);
void partition(ListNode* &front, ListNode*& left, ListNode*& equal, ListNode*& right);
ListNode* concatenate(ListNode* left, ListNode* equal, ListNode* right);

