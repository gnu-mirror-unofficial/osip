/*
  The oSIP library implements the Session Initiation Protocol (SIP -rfc2543-)
  Copyright (C) 2001  Aymeric MOIZARD jack@atosc.org
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <stdlib.h>
#include <stdio.h>

#include <osip/port.h>
#include <osip/list.h>

int list_init(list_t *li) {
  li->nb_elt=0;
  return 0;  /* ok */
}

void
list_special_free(list_t *li,void *(*free_func)(void *))
{
  int pos = 0;
  void *element;
  if (li==NULL) return;
  while (!list_eol(li,pos))
    {
     element = (void *)list_get(li,pos);
     list_remove(li,pos);
     free_func(element);
     sfree(element);
    }
}

void
listofchar_free(list_t *li)
{
  int pos = 0;
  char *chain;
  if (li==NULL) return;
  while (!list_eol(li,pos))
    {
     chain = (char *)list_get(li,pos);
     list_remove(li,pos);
     sfree(chain);
    }
}

int
list_size(list_t *li)
{
  return li->nb_elt;
}

int list_eol(list_t *li,int i) {
  if (i<li->nb_elt)
    return 0; /* not end of list */
  return 1; /* end of list */
}

/* index starts from 0; */
int list_add(list_t *li,void *el,int pos) {
  node_t *ntmp;
  int i = 0;
  if (pos==-1||pos>=li->nb_elt)
    { /* insert at the end  */
    pos = li->nb_elt;
    }

  if (li->nb_elt==0) {

    li->node  = (node_t *)smalloc(sizeof(node_t));
    li->node->element = el;
    li->nb_elt++;
    return li->nb_elt;
  }

  ntmp = li->node;  /* exist because nb_elt>0  */

  if (pos==0)
    {
    li->node         = (node_t *)smalloc(sizeof(node_t));
    li->node->element= el;
    li->node->next   = ntmp;
    li->nb_elt++;
    return li->nb_elt;
    }
  /* pos = 0 insert before first elt  */
  
  while (pos>i+1) 
    {
    i++;
    /* when pos>i next node exist  */
    ntmp = (node_t *)ntmp->next;
    }

  /* if pos==nb_elt next node does not exist  */
  if (pos == li->nb_elt) 
    {
    ntmp->next = (node_t *)smalloc(sizeof(node_t));
    ntmp = (node_t *)ntmp->next;
    ntmp->element = el;
    li->nb_elt++;
    return li->nb_elt;
    }

  /* here pos==i so next node is where we want to insert new node */
  {
  node_t *nextnode = (node_t *)ntmp->next;
  ntmp->next       = (node_t *)smalloc(sizeof(node_t));
  ntmp             = (node_t *)ntmp->next;
  ntmp->element    = el;
  ntmp->next       = nextnode;
  li->nb_elt++;
  }
  return li->nb_elt;
}

/* index starts from 0 */
void *list_get(list_t *li,int pos) {
  node_t *ntmp;
  int i=0;
  if (pos<0||pos>=li->nb_elt)
  /* element does not exist */
    return 0;
    
  
    ntmp = li->node;  /* exist because nb_elt>0 */

  while (pos>i)
    {
    i++;
    ntmp = (node_t *)ntmp->next;
    }
  return ntmp->element;
}

/* return -1 if failed */
int
list_remove(list_t *li,int pos) {

  node_t *ntmp;
  int i=0;
  if (pos<0||pos>=li->nb_elt)
     /* element does not exist */
    return -1;
    
  ntmp = li->node;  /* exist because nb_elt>0 */

  if ((pos == 0))
    { /* special case  */
    li->node = (node_t *)ntmp->next;
    li->nb_elt--;
    sfree(ntmp);
    return li->nb_elt;
    }

  while (pos>i+1)
    {
    i++;
    ntmp = (node_t *)ntmp->next;
    }

  /* if pos==nb_elt next node is the last one */
  if (pos == li->nb_elt)
    {
    sfree(ntmp->next);
    li->nb_elt--;
    return li->nb_elt;
    }

  /* here pos==i so next node is where we want to insert new node */
  {
  node_t *remnode;
  remnode = (node_t *)ntmp->next;
  ntmp->next = ((node_t *)ntmp->next)->next;
  sfree(remnode);
  li->nb_elt--;
  }
  return li->nb_elt;
}

