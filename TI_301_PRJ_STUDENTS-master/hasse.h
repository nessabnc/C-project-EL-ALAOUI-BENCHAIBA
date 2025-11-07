#ifndef __HASSE_H__
#define __HASSE_H__

void removeTransitiveLinks(t_link_array *p_link_array);

typedef struct s_cell{
    int arrival;
    float probability;
    struct s_cell *next;
  }t_cell;

  typedef struct s_list{
    t_cell *head;
    struct s_list *next;
  }t_list;


  typedef struct s_adjlist{
    int size;
    t_list *list;
  }t_adjlist;

t_cell *createCell(int arrival, float probability);
t_list createEmptyList(void);
void addCell(t_list *list, int arrival, float probability);
void displayList(t_list list);
t_adjlist createEmptyAdjList(int size);
void displayAdjList(t_adjlist adj);
t_adjlist readGraph(const char *filename);

#endif