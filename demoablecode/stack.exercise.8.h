// Name: Luke Patrick Dela Cruz
// Course: CMPUT 274 Fa17
// Implementation of the stack data structure as an in-class data structure that was used in directly in this project.

#ifndef STACK_H
#define STACK_H

class Stack {

private:
  struct Node {
    double data[4];
    Node* link;
  };
  // pointer to the head node
  struct Node* head;

public:
  Stack();
  ~Stack();
  bool isEmpty();
  double top();
  void pop();
  void push(double item);
  // Index of the top item in an array within a node
  int toparrIdx;
  void print();
};

/*-------Setting up a char stack ADT-------*/

class CharStack {

private:
  struct Node {
    char data[4];
    Node* link;
  };
  // pointer to the head node
  struct Node* head;

public:
  CharStack();
  ~CharStack();
  bool isCSEmpty();
  char CStop();
  void CSpop();
  void CSpush(char item);
  // Index of the top item in an array within a node
  int toparrIdx;
  void CSprint();
};

#endif
