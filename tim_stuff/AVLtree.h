/* Author: Thinh Le
   AVL Tree for Can Messages
*/

#ifndef AVLTREE_H
#define AVLTREE_H

#include "IO_Driver.h"
#include "IO_EEPROM.h"
#include "IO_CAN.h"

/***************************

		AVL Rotations

Left-Left		When a node is inserted in the right subtree
Left-Right		Node on right side of left branch
Right-Left		Node on left side of right branch
Right-Right		When a node is inserted in the left subtree

***************************/


typedef struct{
	tree * left;
	tree * right;
} * tree;

#endif //AVLTREE_H