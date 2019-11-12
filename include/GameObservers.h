//
// Created by Willi on 11/10/2019.
//

#ifndef COMP_345_PROJ_GAMEOBSERVERS_H
#define COMP_345_PROJ_GAMEOBSERVERS_H

class Player;

class Observer {
    public:
        virtual void update() = 0;
        ~Observer() = default;
    protected:
        Observer() = default;
};

class PhaseObserver: public Observer {
    public:
        explicit PhaseObserver(Player* subject);
        ~PhaseObserver();
        PhaseObserver(const PhaseObserver& toCopy);
        void operator=(const PhaseObserver& rhs);
        void update() override;
        [[nodiscard]]
        inline Player getSubject() const;
        [[nodiscard]]
        inline PlayerState getSubjectState() const;
    private:
        Player* subject;
};

#endif //COMP_345_PROJ_GAMEOBSERVERS_H
