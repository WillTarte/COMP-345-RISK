//
// Created by Willi on 11/10/2019.
//

#ifndef COMP_345_PROJ_GAMEOBSERVERS_H
#define COMP_345_PROJ_GAMEOBSERVERS_H

class Observer {
    public:
        virtual void update() = 0;

    protected:
        Observer() = default;
};

class PhaseObserver: public Observer {
    public:
        PhaseObserver(Player* subject);
        ~PhaseObserver() = default;
        PhaseObserver(const Player& subject);
        void operator=(const Player& rhs);
        void update();
        inline Player getSubject() const { return *subject; };
        inline PlayerState getSubjectState() const { return subject->getPlayerState();}
    private:
        Player* subject;
};

#endif //COMP_345_PROJ_GAMEOBSERVERS_H
