
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

#include "node.h"

VTable VTableNode[] =
{
  {0, 0, (VirtualFunctionPointer) nodeKill}
};

Node * nodeCreate(Node * this, Node * parent)
{
  if (this == 0)
    this = (Node *) malloc(sizeof(Node));

  if (this)
  {
    objCreate((Object *) this);
#ifdef DEBUG_OBJECTS_ON
    commonPrintf("nodeCreate()\n");
#endif
    this->pVTable = VTableNode;

    this->parent = parent;
    this->children = (List *) listCreate(0);
    this->depth = 0;
  }

  return this;
}

void nodeKill(bool dynamic, Node *this)
{
#ifdef DEBUG_OBJECTS_ON
  commonPrintf("~nodeKill()\n");
#endif
  this->pVTable = VTableNode;

  // destructing list members (children)
  Element * elemIt = listFirst(nodeGetChildrenList(this));
  while (elemIt)
  {
    Node *node = (Node *) elemData(elemIt);
    (* node->pVTable[0].pFunc) (TRUE, node);
    elemIt = elemNext(elemIt);
  }

  // destructing list itself
  (* nodeGetChildrenList(this)->pVTable[0].pFunc) (TRUE, nodeGetChildrenList(this));

  objKill(FALSE, (Object *) this);
  if (dynamic) free(this);
}

void nodeDump(Node * this)
{
  commonPrintf("DEPTH            %d\n", nodeGetDepth(this));
  commonPrintf("CHILDREN COUNT   %d\n", listSize(nodeGetChildrenList(this)));
  if (nodeGetParent(this) == 0)
    commonPrintf("TYPE             ROOT NODE\n");
}

Node * nodeGetParent(Node * this)
{
  return this->parent;
}

void nodeAddChild(Node * this, Node * child)
{
  listAppend(this->children, (Object *) child);
}

uint64_t nodeCountAllChildren(Node * this)
{
  uint64_t count = listSize(nodeGetChildrenList(this));
  Element * elemIt = listFirst(nodeGetChildrenList(this));

  while (elemIt)
  {
    Node * node = (Node *) elemData(elemIt);
    count += nodeCountAllChildren(node);
    elemIt = elemNext(elemIt);
  }

  return count;
}

List * nodeGetChildrenList(Node * this)
{
  return this->children;
}

uint16_t nodeGetDepth(Node * this)
{
  return this->depth;
}

void nodeSetDepth(Node * this, uint16_t depth)
{
  this->depth = depth;
}
