#include <iostream>
#include <fstream>
#include <string>

#include "main.h"

void init(String* s) {
    s->data = nullptr;
    s->Mark = ';';
}

void free(String* s) {
    if (s->data) {
        delete[] s->data;
    }
    s->data = nullptr;
}

void copyString(String* dest, const String* src) {
    int length = 0;
    if (src->data) {
        while (src->data[length] != src->Mark) {
            length++;
        }
    }

    if (length > 0 || src->data != nullptr) {
        dest->data = new char[length + 1];
        for (int i = 0; src->data[i] != src->Mark; ++i) {
            dest->data[i] = src->data[i];
        }
        dest->data[length] = dest->Mark;
    } else {
        dest->data = nullptr;
    }
}

void readFromStdString(String* s, const std::string& str) {
    int length = str.length();
    if (length > 0) {
        s->data = new char[length + 1];
        for (int i = 0; i < length; ++i) {
            s->data[i] = str[i];
        }
        s->data[length] = s->Mark;
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
            for (int i = 0; current->data.data[i] != current->data.Mark; ++i) {
                out << current->data.data[i];
            }
            out << "\n";
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
            in.clear();
            in.seekg(0);

            while (std::getline(in, line)) {
                String str;
                init(&str);
                readFromStdString(&str, line);
                addNode(&list, &str);
                free(&str);
            }
            printList(&list, out);
        } else {
            out << "Исходный список пуст\n";
        }
        in.close();
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
            insert.clear();
            insert.seekg(0);
            
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
