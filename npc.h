#ifndef NPC_H
#define NPC_H
#include "character.h"
class Npc: public Character{
    QTimer m_posTimer;
    QTimer m_dirTimer;
    int dir;
public:
    explicit Npc(QGraphicsItem* parent = nullptr);
    ~Npc();
    void updatePos();
    void updateDir(){dir = rand()%4;}
};
#endif
