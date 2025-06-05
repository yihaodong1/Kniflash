#ifndef NPC_H
#define NPC_H
#include "character.h"
class Npc: public Character{
    QTimer m_posTimer;
    QTimer m_dirTimer;
    QTimer m_beattack;
    int dir;
    QGraphicsEllipseItem *beattack;
public:
    explicit Npc(QGraphicsItem* parent = nullptr);
    ~Npc();
    void updatePos();
    void updateDir(){dir = rand()%4;}
    void beAttack(){beattack->setVisible(true);
      m_beattack.stop();
      connect(&m_beattack, &QTimer::timeout,[&](){
            m_beattack.stop();
            beattack->setVisible(false);
          });
      m_beattack.start(1000);
    }
};
#endif
