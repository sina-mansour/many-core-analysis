#include "sorter.h"

Sorter::Sorter()
{
}

void Sorter::portSorter(Graph *graph)
{
    int actorCount;
    actorCount = graph->getActorCount();
    for (int i=0; i<actorCount; i++)
    {
        Actor* tmpA;
        tmpA = graph->getIthActor(i);
        tmpA->scoreSort();
    }
}

void Sorter::portValueSorter(Graph *graph)
{
    int actorCount;
    actorCount = graph->getActorCount();
    for (int i=0; i<actorCount; i++)
    {
        Actor* tmpA;
        tmpA = graph->getIthActor(i);
        tmpA->valueSort();
    }
}
