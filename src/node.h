
/* Copyright 2009 Ralf Schmitt
 *
 * This file is part of volsort.
 *
 * volsort is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * volsort is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with volsort. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef NODE_H
#define NODE_H

#include "list.h"

typedef struct node Node;

struct node
{
  VTable * pVTable;
  Node * parent;
  List * children;
  uint16_t depth;
};

void nodeKill(bool dynamic, Node * this);

Node * nodeCreate(Node * this, Node * parent);

List * nodeGetChildrenList(Node * this);

void nodeAddChild(Node * this, Node * child);

uint64_t nodeCountAllChildren(Node * this);

uint16_t nodeGetDepth(Node * this);

void nodeSetDepth(Node * this, uint16_t depth);

Node * nodeGetParent(Node * this);

void nodeDump(Node * this);

#endif
