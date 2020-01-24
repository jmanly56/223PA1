#ifndef LIST_H
#define LIST_H
#include <iostream>
#include <sstream>
#include <utility>

/*
 * Author: Jonathan Manly
 * Description:
 *      Contains definitions for a Linked List(List)
 *      and its accompanying Node class.
 */

/*
 * Node helper class for the List class.
 * The constructor allows both no arguements as well as
 * directly specifying data without having two seperate
 * constructor definitions.
 * Data is left public because this is essentially a struct
 * with an explicit constructor.
 *
 */

template <typename T, typename U>
class Node
{
    public:
        Node(T data1 = nullptr, U data2 = nullptr)
        {
                this->data1 = data1;
                this->data2 = data2;
                next = nullptr;
        }
        T data1;
        U data2;
        Node *next;
        bool operator==(Node<T, U> &rhs)
        {
                return rhs.data1 == data1 && rhs.data2 == data2;
        }
};

/*
 * Template Class List
 * Basic singly linked list.
 *
 */
template <typename T, typename U>
class List
{
    public:
        /* Functions  */
        List()
        {
                head = nullptr;
                num_nodes = 0;
        }
        ~List()
        {
                cleanup();
        }
        bool isEmpty()
        {
                return head == nullptr;
        }

        /*
         * Adds a node to the front of the list
         * and increases the size of the list.
         * The data could technically be nullptr,
         * but doing so would be mostly useless.
         *
         * Example:
         * my_list.prepend("foo", "bar")
         *
         */
        void prepend(T data1, U data2)
        {
                Node<T, U> *ptr = new Node<T, U>(data1, data2);
                if (head) {
                        ptr->next = head;
                        head = ptr;
                } else {
                        head = ptr;
                }
                num_nodes++;
        }

        /*
         * Removes the FIRST node matching the specified data.
         */
        bool remove(T data1, U data2)
        {
                Node<T, U> *ptr = head;
                Node<T, U> *prev = nullptr, *match = nullptr;
                Node<T, U> comp(data1, data2); // tmp node to compare with.
                while (ptr != nullptr) {
                        if (*ptr == comp) {
                                match = ptr;
                                break;
                        }
                        prev = ptr;
                        ptr = ptr->next;
                }
                if (match != nullptr) {
                        if (prev == nullptr) {
                                ptr = head->next;
                                delete head;
                                head = ptr;
                        } else {
                                prev->next = match->next;
                                delete match;
                        }
                        num_nodes--;
                        return true;
                } else
                        return false;
        }
        bool remove(T data1)
        {
                Node<T, U> *ptr = head;
                Node<T, U> *prev = nullptr, *match = nullptr;
                while (ptr != nullptr) {
                        if (ptr->data1 == data1) {
                                match = ptr;
                                break;
                        }
                        prev = ptr;
                        ptr = ptr->next;
                }
                if (match != nullptr) {
                        if (prev == nullptr) {
                                ptr = head->next;
                                delete head;
                                head = ptr;
                        } else {
                                prev->next = match->next;
                                delete match;
                        }
                        num_nodes--;
                        return true;
                } else
                        return false;

                
        }

        /*
         * Finds the FIRST match of the given data and
         * returns a constant pointer. If no match
         * found, it returns nullptr.
         */
        Node<T, U> *find(T data1, U data2) const
        {
                Node<T, U> *ptr = head;
                Node<T, U> *match = nullptr;
                Node<T, U> comp(data1, data2); // tmp node to compare with.
                while (ptr != nullptr) {
                        if (*ptr == comp) {
                                match = ptr;
                                break;
                        }
                        ptr = ptr->next;
                }
                return match;
        }

        /*
         * Uses a stringstream to create a
         * newline seperated list of CSV values.
         * It assumes that the data within the node will
         * overload the << operator if it is a non-standard
         * type. It then outputs the string created by ss.
         */

        std::string toString() const
        {
                std::stringstream ss;
                ss << "";
                Node<T, U> *ptr = head;
                while (ptr != NULL) {
                        ss << ptr->data1 << ",\"" << ptr->data2 << "\""
                           << std::endl;
                        ptr = ptr->next;
                }
                return ss.str();
        }
        size_t getSize() const
        {
                return num_nodes;
        }
        Node<T, U> *at(unsigned int index)
        {
                Node<T, U> *ptr = head;
                for (size_t i = 0; i < getSize(); i++) {
                        if (i == index)
                                return ptr;
                        else
                                ptr = ptr->next;
                }
                return nullptr;
        }

    private:
        /* Variables */
        Node<T, U> *head;
        size_t num_nodes;

        /* Functions */
        void cleanup()
        {
                Node<T, U> *tmp, *ptr = head;
                while (ptr != nullptr) {
                        tmp = ptr->next;
                        delete ptr;
                        ptr = tmp;
                }
        }
};

#endif // LIST_H
