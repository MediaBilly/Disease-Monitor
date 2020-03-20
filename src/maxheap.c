#include <stdlib.h>
#include "../headers/maxheap.h"
#include "../headers/patientRecord.h"
#include "../headers/utilities.h"

// Define data's structure

typedef struct heapnode *HeapNode;

struct heapdata {
  string key;
  unsigned int value;
};

struct heapnode {
  HeapNode left,right,parent;
	HeapData data;
};

struct maxheap {
  HeapNode root;
  HeapNode last;
};

int MaxHeap_Create(MaxHeap *heap) {
  // Allocate memory for the heap
  if ((*heap = (MaxHeap)malloc(sizeof(struct maxheap))) == NULL) {
    not_enough_memory();
    return FALSE;
  }
  (*heap)->root = NULL;
  (*heap)->last = NULL;
  return TRUE;
}

// Swaps values between 2 heap nodes
void swap_values(HeapNode x,HeapNode y) {
  HeapData tmp = x->data;
  x->data = y->data;
  y->data = tmp;
}

enum Direction{Left,Right};

// Gets next position for new insertion in the binary heap
HeapNode next_available_leaf(HeapNode last,enum Direction *direction) {
  // More than 1 total nodes
  if (last->parent != NULL) {
    // last is left child so return it's parent's right position
    if (last == last->parent->left) {
      *direction = Right;
      return last->parent;
    } else {
      // Otherwise traverse upwards and downwards until we reach the next left positon
      while (last->parent != NULL && last == last->parent->right) {
        last = last->parent;
      }
      if (last->parent != NULL) {
        last = last->parent->right;
      }
      while (last->left != NULL) {
        last = last->left;
      }
      *direction = Left;
      return last;
    }
  } else {
    // Only root exists(last is root) so return it's left position
    *direction = Left;
    return last;
  }
}

string MaxHeapData_GetKey(HeapData data) {
  return data->key;
}

unsigned int MaxHeapData_GetValue(HeapData data) {
  return data->value;
}

int MaxHeapData_Destroy(HeapData *data) {
  if (*data != NULL) {
    free(*data);
    *data = NULL;
    return TRUE;
  } else {
    return FALSE;
  }
}

int MaxHeap_Insert(MaxHeap heap,string key,unsigned int value) {
  // Allocate space for the new node
  HeapNode node;
  if ((node = (HeapNode)malloc(sizeof(struct heapnode))) == NULL) {
    not_enough_memory();
    return FALSE;
  }
  // Allocate space for the data
  HeapData data;
  if ((data = (HeapData)malloc(sizeof(struct heapdata))) == NULL) {
    not_enough_memory();
    free(node);
    return FALSE;
  }
  // Initialize it's attributes
  data->key = key;
  data->value = value;
  node->data = data;
  node->left = node->right = NULL;
  // Insert it to the last element position
  if (heap->root != NULL) {
    enum Direction direction;
    HeapNode nextPos = next_available_leaf(heap->last,&direction);
    switch (direction) {
      case Left:
        nextPos->left = node;
        break;
      case Right:
      default:
        nextPos->right = node;
        break;
    }
    node->parent = nextPos;
  } else {
    heap->root = node;
    node->parent = NULL;
  }
  heap->last = node;
  // Move up to the appropriate position (parent's value must be >= and all children must be <= to it)
  HeapNode last = heap->last;
  while (last->parent != NULL && last->data->value > last->parent->data->value) {
    swap_values(last,last->parent);
    last = last->parent;
  }
  return TRUE;
}

// Removes last node
void remove_last(MaxHeap heap) {
  // At least 2 nodes exist
  if (heap->last->parent != NULL) {
    HeapNode toDel = heap->last;
    if (heap->last == heap->last->parent->right) {
      heap->last = heap->last->parent->left;
      toDel->parent->right = NULL;
    } else {
      while (heap->last->parent != NULL && heap->last == heap->last->parent->left) {
        heap->last = heap->last->parent;
      }
      if (heap->last->parent != NULL) {
        heap->last = heap->last->parent->left;
      }
      while (heap->last->right != NULL) {
        heap->last = heap->last->right;
      }
      toDel->parent->left = NULL; 
    }
    free(toDel);
  } else {
    // Only root exists
    free(heap->root);
    heap->last = heap->root = NULL;
  }
}

HeapData MaxHeap_ExtractMax(MaxHeap heap) {
  // No elements to delete(empty heap)
  if (heap->root == NULL) {
    return NULL;
  }
  // Delete min element(root)
  HeapData ret = heap->root->data;
  // Place last element to it's place
  heap->root->data = heap->last->data;
  remove_last(heap);
  // Max heapify starting at root to bring the last element's data to the correct position
  HeapNode current = heap->root;
  HeapNode maxnode;
  while(current != NULL) {
    maxnode = current;
    if (current->left != NULL && current->left->data->value > maxnode->data->value) {
      maxnode = current->left;
    }
    if (current->right != NULL && current->right->data->value > maxnode->data->value) {
      maxnode = current->right;
    }
    if (maxnode != current) {
      swap_values(current,maxnode);
      current = maxnode;
    } else {
      break;
    }
  }
  return ret;
}

int MaxHeap_Destroy(MaxHeap *heap) {
  // Free all allocated memory
  if (*heap != NULL) {
    // Destroy all remaining nodes
    while ((*heap)->root != NULL) {
      HeapData toDel = MaxHeap_ExtractMax(*heap);
      MaxHeapData_Destroy(&toDel);
    }
    free(*heap);
    *heap = NULL;
    return TRUE;
  } else {
    return FALSE;
  }
}