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
        inline Player getSubject() const;
        inline PlayerState getSubjectState() const;
    private:
        Player* subject;
};

class StatsObserver: public Observer {
    public:
        explicit StatsObserver(Player* subject);
        ~StatsObserver();
        StatsObserver(const StatsObserver& toCopy);
        void operator=(const StatsObserver& rhs);
        void update() override;
        static void printTable();
        static void printStats();
    private:
      Player* subject;
};
#endif //COMP_345_PROJ_GAMEOBSERVERS_H
