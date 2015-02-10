#ifndef NODE_H
#define NODE_H

#include "stdlib.h"

class CNode
{
public:
   bool active;
   bool open;
   bool empty;
   CNode* parrent;
   int c,r;
   int x,y;
   int G,F,H;
   CNode(bool _active) : active(_active), empty(_active){ F=0; H=0; G=0; open=false; parrent=0; };
};

#endif
