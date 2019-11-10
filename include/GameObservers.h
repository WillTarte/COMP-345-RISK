//
// Created by Willi on 11/10/2019.
//

#ifndef COMP_345_PROJ_GAMEOBSERVERS_H
#define COMP_345_PROJ_GAMEOBSERVERS_H

#include "Player.h"

class Observer {
    public:
        virtual void update() = 0;
        ~Observer() = default;
    protected:
        Observer() = default;
};

class PhaseObserver: public Observer {
    public:
        PhaseObserver(Player* subject);
        ~PhaseObserver();
        PhaseObserver(const PhaseObserver& toCopy);
        void operator=(const PhaseObserver& rhs);
        void update();
        inline Player getSubject() const { return *subject; };
        inline PlayerState getSubjectState() const { return subject->getPlayerState();}
    private:
        Player* subject;
};

#endif //COMP_345_PROJ_GAMEOBSERVERS_H
