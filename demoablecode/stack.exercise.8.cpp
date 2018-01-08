// Course: CMPUT 274 Fa17
// Implementation of the stack data structure as an in-class data structure that was used in directly in this project.

#include <Arduino.h>
#include <string.h>
#include "stack.exercise.8.h"

/*-------Modification of stack to use double values-------*/

// initializes a stack via a linked list
Stack::Stack() {
  // pointer to the head node that is initialized at NULL for an empty start
  head = NULL;
  // toparrIdx always holds the position of last item in
  toparrIdx = 0;
}

// obliterates link list of the stack ADT
Stack::~Stack() {
  Node* temporary;
  temporary = head;
  while (temporary != NULL) {
    delete (head);
    head = temporary->link;
    temporary = head;
  }
}

// The stack is empty if and only if the head is a null pointer
bool Stack::isEmpty() {
  return head == NULL;
}

// Returns last in
double Stack::top() {
  if (isEmpty()) {return 0;}
  if (toparrIdx == 4) {toparrIdx--;}
  return head->data[toparrIdx];
}

// First out
void Stack::pop() {
  if (isEmpty()) {return;}
  if (toparrIdx < 4 && toparrIdx > 0) {
    Node* temporary;
    temporary = head;
    temporary -> data[toparrIdx] = 0; // element deletion
    toparrIdx--;
  }
  else if (toparrIdx == 0) {
    Node* temporary;
    temporary = head;
    head = head -> link;
    delete (temporary);
    if (head != NULL) { // since you can pop your way to being empty
      toparrIdx = 3; // the top of the stack is always the last array item
                     // at the head of the linked list
    }
  }
}
// Last in
void Stack::push(double item) {
  if (isEmpty()) {
    Node* temporary = new Node();
    temporary -> data[toparrIdx] = item;
    temporary -> link = head; // get the address of the last node
    head = temporary; // insert at the beginning
  }
  else if (toparrIdx < 3) { // ordinary case
    toparrIdx++;
    Node* temporary = head;
    temporary -> data[toparrIdx] = item;
  }
  else if (toparrIdx == 3) {
    // new node is needed since the old node's array is filled
    toparrIdx = 0;
    Node* temporary = new Node();
    temporary -> data[toparrIdx] = item;
    temporary -> link = head;
    head = temporary;
  }
}
// for debugging purposes
void Stack::print() {
  Node* temporary = head;
  temporary = head;
  while (temporary != NULL) {
    for (int i = 0; i < 4; i++) {
      Serial.print(temporary -> data[i]);
      Serial.println();
    }
    temporary = temporary -> link;
  }
}

/*-------Setting up a char stack ADT-------*/

// initializes a stack via a linked list
CharStack::CharStack() {
  // pointer to the head node that is initialized at NULL for an empty start
  head = NULL;
  // toparrIdx always holds the position of last item in
  toparrIdx = 0;
}

// obliterates link list of the stack ADT
CharStack::~CharStack() {
  Node* temporary;
  temporary = head;
  while (temporary != NULL) {
    delete (head);
    head = temporary->link;
    temporary = head;
  }
}

// The stack is empty if and only if the head is a null pointer
bool CharStack::isCSEmpty() {
  return head == NULL;
}

// Returns last in
char CharStack::CStop() {
  if (isCSEmpty()) {return 0;}
  if (toparrIdx == 4) {toparrIdx--;}
  return head->data[toparrIdx];
}

// First out
void CharStack::CSpop() {
  if (isCSEmpty()) {return;}
  if (toparrIdx < 4 && toparrIdx > 0) {
    Node* temporary;
    temporary = head;
    temporary -> data[toparrIdx] = 0; // element deletion
    toparrIdx--;
  }
  else if (toparrIdx == 0) {
    Node* temporary;
    temporary = head;
    head = head -> link;
    delete (temporary);
    if (head != NULL) { // since you can pop your way to being empty
      toparrIdx = 3; // the top of the stack is always the last array item
                     // at the head of the linked list
    }
  }
}
// Last in
void CharStack::CSpush(char item) {
  if (isCSEmpty()) {
    Node* temporary = new Node();
    temporary -> data[toparrIdx] = item;
    temporary -> link = head; // get the address of the last node
    head = temporary; // insert at the beginning
  }
  else if (toparrIdx < 3) { // ordinary case
    toparrIdx++;
    Node* temporary = head;
    temporary -> data[toparrIdx] = item;
  }
  else if (toparrIdx == 3) {
    // new node is needed since the old node's array is filled
    toparrIdx = 0;
    Node* temporary = new Node();
    temporary -> data[toparrIdx] = item;
    temporary -> link = head;
    head = temporary;
  }
}
// for debugging purposes
void CharStack::CSprint() {
  Node* temporary = head;
  temporary = head;
  while (temporary != NULL) {
    for (int i = 0; i < 4; i++) {
      Serial.print(temporary -> data[i]);
      Serial.println();
    }
    temporary = temporary -> link;
  }
}
