#ifndef _SINGLELINKEDLIST_H__
#define _SINGLELINKEDLIST_H__

#include <iostream>

using namespace std;
template <typename DataType>
class SingleLinkedList
{
private:
    struct Node
    {
        Node *next;
        DataType data;        
    };

    int size;
    Node *head;
    Node *tail;
    
public:
    SingleLinkedList()
    {
        size = 0;
        head = NULL;
        tail = NULL;
    }
    
    SingleLinkedList(Node *h)
    {
        size = 0;
        head = h;
        head->next = tail;
        tail = NULL;
        
        for (Node *n = head; n != NULL; n = n->next){
            size++;
        }    
    }
    
    ~SingleLinkedList()
    {
        deleteAll();
    }
    
    void insert(const DataType &d)
    {
        Node *n = new Node;
        n->data = d;
        
        if (size == 0)
        {        
            head = n;
            tail = n;
            head->next = tail;
            tail->next = NULL;
        }
        else
        {
            tail->next = n;
            n->next = NULL;
            tail = n;
        }
        size++;
    }
    void remove(const int index)
    {
        if (size == 0){
        cout << "List is empty" << endl;
        }
        else if (index >= size || index < 0){
            cout << "Invalid index number" << endl;
        }
        else
        {
            Node *selectedItem;
            if (index == 0)
            {
                selectedItem = head;
                head = head->next;
            }
            else
            {
                Node *prev = head;
                for (int i=1;i<index;i++){
                    prev = prev->next;
                }
                selectedItem = prev->next;
                prev->next = selectedItem->next;
            }
            #ifdef _DEBUGMODE_
            cout << "Deleting:" << endl;
            #endif
            
            // remove item
            size--;        
            delete selectedItem;   
        }
    }
    void deleteAll()
    {
        while(!isEmpty()){
            remove(0);
        }
    }    
    bool isEmpty()
    {
        return size == 0;
    }
    string getValue(const string &token)
    {
        string r;
        Node *n = head;
        for (int i=0;i<size;i++)
        {
            if (n->data.token == token){
                r = n->data.value;
            }
            else{
                n = n->next;
            }
        }
        return r;        
    }
    void setValue(const string &token, const string &value)
    {
        Node *n = head;
        for (int i=0;i<size;i++)
        {
            if (n->data.token == token){
                n->data.value = value;
            }
            else{
                n = n->next;
            }
        }        
    }
    void displayAll()
    {
        cout << "size =" << size << endl;
        Node *n = head;
        for (int i=0;i<size;i++)
        {
            n->data.print();
            n = n->next;
        }
    }
};

#endif