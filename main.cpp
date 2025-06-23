#include <iostream>
#include <fstream>
#include <string>

#include "main.h"

void init(String* s) {
    s->data = nullptr;
    s->length = 0;
}

void free(String* s) {
    if (s->data) {
        delete[] s->data;
    }
    s->data = nullptr;
    s->length = 0;
}

void copyString(String* dest, const String* src) {
    dest->length = src->length;
    if (dest->length > 0) {
        dest->data = new char[dest->length + 1];
        for (int i = 0; i < dest->length; ++i) {
            dest->data[i] = src->data[i];
        }
        dest->data[dest->length] = '\0';
    } else {
        dest->data = nullptr;
    }
}

void readFromStdString(String* s, const std::string& str) {
    s->length = str.length();
    if (s->length > 0) {
        s->data = new char[s->length + 1];
        for (int i = 0; i < s->length; ++i) {
            s->data[i] = str[i];
        }
        s->data[s->length] = '\0';
    } else {
        s->data = nullptr;
    }
}

void initList(LinkedList* list) {
    list->head = nullptr;
}

void addNode(LinkedList* list, const String* str) {
    Node* newNode = new Node;
    init(&newNode->data);
    copyString(&newNode->data, str);
    newNode->next = nullptr;

    if (list->head == nullptr) {
        list->head = newNode;
    } else {
        Node* current = list->head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }
}

void freeList(LinkedList* list) {
    Node* current = list->head;
    while (current != nullptr) {
        Node* next = current->next;
        free(&current->data);
        delete current;
        current = next;
    }
    list->head = nullptr;
}

void insertAfterLast(LinkedList* list, int count, String* newElements) {
    for (int i = 0; i < count; ++i) {
        addNode(list, &newElements[i]);
    }
}

void printList(const LinkedList* list, std::ofstream& out) {
    Node* current = list->head;
    while (current != nullptr) {
        if (current->data.data) {
            out << current->data.data << "\n";
        }
        current = current->next;
    }
}

int main() {
    LinkedList list;
    initList(&list);
    
    std::ofstream out("output.txt");
    if (!out) {
        std::cerr << "Cannot open output file: " << "output.txt" << std::endl;
        return 1;
    }

    std::ifstream in("input.txt");
    if (in) {
        out << "Исходный список:\n";
        std::string line;
        int count = 0;
        while (std::getline(in, line)) {
            count++;
        }

        if (count > 0) {
            in.close();
            in.open("input.txt");

            while (std::getline(in, line)) {
                String str;
                init(&str);
                readFromStdString(&str, line);
                addNode(&list, &str);
                free(&str);
            }
            in.close();
            printList(&list, out);
        } else {
            out << "Исходный список пуст\n";
        }
    } else {
        out << "Исходный список не найден\n";
    }

    std::ifstream insert("insert.txt");
    if (insert) {
        std::string line;
        int count = 0;
        while (std::getline(insert, line)) {
            count++;
        }
        
        if (count > 0) {
            insert.close();
            insert.open("insert.txt");
            
            String* newElements = new String[count];
            for (int i = 0; i < count; ++i) {
                std::getline(insert, line);
                init(&newElements[i]);
                readFromStdString(&newElements[i], line);
            }
            
            insertAfterLast(&list, count, newElements);
            out << "\nПосле вставки " << count << " элементов:\n";
            printList(&list, out);
            
            for (int i = 0; i < count; ++i) {
                free(&newElements[i]);
            }
            delete[] newElements;
        } else {
            out << "\nНет элементов для вставки (файл пуст)\n";
        }
        insert.close();
    } else {
        out << "\nФайл для вставки не найден\n";
    }

    freeList(&list);
    out.close();
    return 0;
}
