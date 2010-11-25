/*The LinkedList class of the ImageEvolverQt program
 * Copyright (C) 2009-2010  Aaron Fan
 * Version 3.3
 *
 *This program is free software: you can redistribute it and/or modify
 *it under the terms of the GNU General Public License as published by
 *the Free Software Foundation, either version 3 of the License, or
 *(at your option) any later version.
 *
 *This program is distributed in the hope that it will be useful,
 *but WITHOUT ANY WARRANTY; without even the implied warranty of
 *MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *GNU General Public License for more details.
 *
 *You should have received a copy of the GNU General Public License
 *along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <math.h>

namespace core
{
    class LinkedList;
    class ListNode;
}

template <typename T>
class LinkedList
{
    public:
        LinkedList(){numOfNodes = 0; firstNode = 0; lastNode = 0; currentNode = 0;}
        ~LinkedList();
        void append(T *in);
        void prepend(T *in);
        void insert(int index, T *in);
        void removeAndDelete(int index);
        T * first();
        T * last();
        T * remove(int index);
        T * get(int index);
        void removeAll();
        int size();
        template <typename P> class ListNode
        {
            public:
                ListNode(ListNode<P> *p, ListNode<P> *n, P *v)
                {
                    prev = p; next = n; value = v;
                }
            ListNode<P> *prev, *next;
            P *value;
        };

    private:
        void moveCurrentTo(int input);
        ListNode<T> *firstNode, *lastNode, *currentNode;
        int numOfNodes, currentNodeIndex;	
};

template <typename T>
LinkedList<T>::~LinkedList()
{
    for (int x = 0; x < numOfNodes; x++)
    {
        currentNode = firstNode->next;
        delete firstNode->value;
        delete firstNode;
        firstNode = currentNode;
    }
}

template <typename T>
void LinkedList<T>::append(T *in)
{
    lastNode = new ListNode<T>(lastNode, 0, in);
    if (lastNode->prev != 0)
        lastNode->prev->next = lastNode;
    else
    {
        firstNode = lastNode;
        currentNode = firstNode;
        currentNodeIndex = 0;
    }
    numOfNodes++;

}

template <typename T>
void LinkedList<T>::prepend(T *in)
{
    firstNode = new ListNode<T>(0, firstNode, in);
    currentNodeIndex++;

    if (firstNode->next != 0)
        firstNode->next->prev = firstNode;
    else
    {
        lastNode = firstNode;
        currentNode = firstNode;
        currentNodeIndex = 0;
    }
    numOfNodes++;
}

template <typename T>
void LinkedList<T>::insert(int index, T *in)
{
    if (numOfNodes == 0 || index == 0)
        prepend(in);
    else if (index == numOfNodes)
        append(in);
    else
    {
        moveCurrentTo(index);

        currentNode->prev->next = new ListNode<T>(currentNode->prev, currentNode, in);
        currentNode->prev = currentNode->prev->next;

        currentNode = currentNode->prev;

        numOfNodes++;
    }
}

template <typename T>
void LinkedList<T>::removeAndDelete(int index)
{
    T* value = remove(index);
    delete value;
}

template <typename T>
T * LinkedList<T>::remove(int index)
{
    moveCurrentTo(index);

    T* value = currentNode->value;
    ListNode<T> *temp = currentNode;

    if (index == 0)
    {
        firstNode = firstNode->next;
        currentNode = firstNode;
    }

    else if (index == numOfNodes-1)
    {
        lastNode = lastNode->prev;
        currentNode = lastNode;
        currentNodeIndex--;
    }

    else
    {
        currentNode->next->prev = currentNode->prev;
        currentNode->prev->next = currentNode->next;
        currentNode = currentNode->next;
    }

    numOfNodes--;

    delete temp;

    return value;
}

template <typename T>
T * LinkedList<T>::get(int index)
{
    moveCurrentTo(index);
    return currentNode->value;
}

template <typename T>
T * LinkedList<T>::first()
{
    return firstNode->value;
}

template <typename T>
T * LinkedList<T>::last()
{
    return lastNode->value;
}

template <typename T>
int LinkedList<T>::size()
{
    return numOfNodes;
}

template <typename T>
void LinkedList<T>::removeAll()
{
    for (int x = 0; x < numOfNodes; x++)
    {
        currentNode = firstNode->next;
        delete firstNode->value;
        delete firstNode;
        firstNode = currentNode;
    }
    firstNode = 0;
    currentNode = 0;
    lastNode = 0;
    numOfNodes = 0;
    currentNodeIndex = 0;
}

template <typename T>
void LinkedList<T>::moveCurrentTo(int index)
{
    int dif1 = index;
    int dif2 = fabs((float)(currentNodeIndex - index));
    int dif3 = numOfNodes - index;

    int pathToTake =  dif1 <= dif2 && dif2 <= dif3 ? 1 :
                      dif2 <= dif1 && dif1 <= dif3 ? 2 :
                                                   3;

    int startInd;
    int change;

    if (pathToTake == 1)
    {
        currentNode = firstNode;
        startInd = 0;
        change = 1;
    }

    if (pathToTake == 2)
    {
        startInd = currentNodeIndex;
        change = index > currentNodeIndex ? 1 : -1;
    }

    if (pathToTake == 3)
    {
        currentNode = lastNode;
        startInd = numOfNodes-1;
        change = -1;
    }

    for (int x = 0; x < fabs((float)(startInd - index)); x++)
    {
        if (change == 1)
            currentNode = currentNode->next;
        else
            currentNode = currentNode->prev;
    }

    currentNodeIndex = index;
}

#endif // LINKEDLIST_H
