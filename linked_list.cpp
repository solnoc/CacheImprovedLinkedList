#include <iostream>
#include <chrono>
#include <stdlib.h>

long long MAX_NODES = 512000;

class linkedList
{
public:
    linkedList();

public:
    struct Node
    {
        Node* next;
        unsigned int payload;
    };

public:
    Node* createNode(void* allocated_memory, Node* next = NULL, unsigned int payload = 0);
    void* removeNode(Node* node);
    void insertNode(Node* node, Node* previous = NULL);

public:
    Node* entrance;
    void* start_of_buffer;
    void* buffer_index;

};


int main(int argc, char** argv)
{
    if(argc == 2)
    {
        try
        {
            MAX_NODES = std::atoll(argv[1]);
        }
        catch(...)
        {
            std::cout << "Number expected!" << std::endl;
        }
    }

    linkedList cacheImprovedList;
    linkedList classicList;

    linkedList::Node* initialNode = cacheImprovedList.createNode(cacheImprovedList.start_of_buffer);
    cacheImprovedList.insertNode(initialNode);
    cacheImprovedList.entrance = initialNode;
    linkedList::Node* previous = initialNode;
    for(int i=1; i < MAX_NODES; i++)
    {
        linkedList::Node* node = cacheImprovedList.createNode(((linkedList::Node*)cacheImprovedList.start_of_buffer) +  i);
        cacheImprovedList.insertNode(node, previous);
        previous = node;
    }

    initialNode = classicList.createNode(malloc(sizeof(linkedList::Node)));
    classicList.insertNode(initialNode);
    classicList.entrance = initialNode;
    previous = initialNode;
    for(int i=1; i < MAX_NODES; i++)
    {
        linkedList::Node* node = classicList.createNode(malloc(sizeof(linkedList::Node)));
        classicList.insertNode(node, previous);
        previous = node;
    }

    linkedList::Node* it = cacheImprovedList.entrance;
    auto start = std::chrono::high_resolution_clock::now();

    while(it != NULL)
        it = it->next;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout<<"Cache improved list has been traveresed in " << duration.count() << " seconds." << std::endl;

    it = classicList.entrance;
    start = std::chrono::high_resolution_clock::now();

    while(it != NULL)
        it = it->next;

    end = std::chrono::high_resolution_clock::now();
    duration = end - start;

    std::cout<<"Classical list has been traveresed in " << duration.count() << " seconds." << std::endl;
    return 0;
}

linkedList::linkedList()
{
    buffer_index = malloc(sizeof(Node) * MAX_NODES);
    start_of_buffer = buffer_index;

    //std::cout<<"Constructed."<<std::endl;
}


linkedList::Node* linkedList::createNode(void* allocated_memory, Node* next, unsigned int payload)
{
    ((Node*)allocated_memory)->next = next;
    ((Node*)allocated_memory)->payload = payload;

    //std::cout<<"Created Node."<<std::endl;
    return (Node*)allocated_memory;
}

void* linkedList::removeNode(Node* node)
{
    Node* it = entrance;
    while(it->next != NULL && it->next != node)
    {
        it = it->next;
    }
    if(it->next != NULL)
    {
        it->next = it->next->next;
    }
    return (void*)node;
}

void linkedList::insertNode(Node* node, Node* previous)
{
    if(previous == NULL)
    {
        return;
    }
    node->next = previous->next;
    previous->next = node;
}
