#include <iostream>
#include <iomanip>
#include <algorithm>
#include "Warrior.h"

#define BLUE 0
#define RED 1

class Place{
    protected:
    Warrior* wariRed, * wariBlue;

    public:
    Place() : wariRed(nullptr), wariBlue(nullptr){}
    virtual Warrior* WariOut(int){};
    virtual void WariIn(int, Warrior*, int, int){};
    void WariReport(int, int);//
    void LionRun(int, int);//
};

class City: public Place{
    private:
    int index;

    public:
    City() : Place(){};
    void WolfRob(int, int);
    void Battle(int, int);
    void SetIndex(int i){ index = i; }
    Warrior* WariOut(int);
    void WariIn(int, Warrior*, int, int);
};

class Headquarter: public Place{
    private:
    int* wariLife,* wariAttack;
    int numberWarrior;
    int elements;
    int loyalty;
    int type;

    public:
    Headquarter(int*, int*, int, int, int);
    bool IsOccupied();
    void CreateWarrior(int, int);
    void Report(int, int);
    Warrior* WariOut(int);
    void WariIn(int, Warrior*, int, int);
};

class Game{
    private:
    Headquarter headBlue, headRed;
    City* cities;
    int numberCity;
    int timeLimit, timeCurrent;

    public:
    Game(int m, int n, int k, int t, int* life, int* atk);
    void Run();
    int NextHour();
};

class GerneralData{
    const static int blueOrder[5];
    const static std::string wariNames[5];
    const static std::string weaponNames[3];
}

const int GerneralData::blueOrder[5] = {1, 4, 3, 0, 2};
const std::string GerneralData::wariNames[5] = 
    {"iceman", "lion", "wolf", "ninja", "dragon"};
const std::string GerneralData::weaponNames[3] =
    {"sword", "bomb", "arrow"};

Game::Game(int m, int n, int k, int t, int* life, int* atk)
    : numberCity(n), timeLimit(t), 
    headBlue(life, atk, BLUE, m, k), headRed(life, atk, RED, m, k){
    cities = new City[n];
    for(int i=0; i<n; ++i){
        cities[i].SetIndex(i + 1);
    }
}

void Game::Run(){
    while(timeCurrent < timeLimit){
        if(NextHour()) break;
        timeCurrent ++;
    }
}

int Game::NextHour(){
    int time = 0;
    headRed.CreateWarrior(timeCurrent, time);
    headBlue.CreateWarrior(timeCurrent, time);

    time = 5;
    headRed.LionRun(timeCurrent, time);
    for(auto& city : cities){
        city[i].LionRun(timeCurrent, time);
    }
    headBlue.LionRun(timeCurrent, time);

    time = 10;
    std::function<void(Place&, Place&, int)> WariMov = 
    [&](Place& from, Place& to, int type){
        auto wari = from.WariOut(type);
        if(wari != nullptr){
            to.WariIn(type, wari, timeCurrent, time);
        }
    };
    int finished = 0;
    WariMov(cities[0], headRed, BLUE);
    if(headRed.IsOccupied()) finished = 1;
    WariMov(headRed, cities[0], RED);
    for(int i=1; i<numberCity - 1; ++i){
        WariMov(cities[i], cities[i-1], BLUE);
        WariMov(cities[i-1], cities[i], RED);
    }
    WariMov(headBlue, cities[numberCity-1], BLUE);
    WariMov(cities[numberCity-1], headBlue, RED);
    if(headBlue.IsOccupied()) finished = 1;
    if(finished) return 1;

    time = 35;
    for(auto& city : cities){
        city.WolfRob(timeCurrent, time);
    }

    time = 40;
    for(auto& city : cities){
        city.Battle(timeCurrent, time);
    }

    time = 50;
    headRed.Report(timeCurrent, time);
    headBlue.Report(timeCurrent, time);

    time = 55;
    headRed.WariReport(timeCurrent, time);
    for(auto& city : cities){
        city.WariReport(timeCurrent, time);
    }
    headBlue.WariReport(timeCurrent, time);

    return 0;
}

Headquarter::Headquarter(int* life, int* atk, int type, int elements, int loyalty)
    : wariLife(life), wariAttack(atk), type(type), elements(elements),
    loyalty(loyalty), status(0), numberWarrior(0), Place() {
}

void Headquarter::CreateWarrior(int timeCurrent, int time){
    //红方司令部按照iceman、lion、wolf、ninja、dragon 的顺序制造武士。
    //蓝方司令部按照lion、dragon、ninja、iceman、wolf 的顺序制造武士。
    auto& wari = type == RED ? wariRed : wariBlue;
    int index = type == RED ? numberWarrior % 5 : 
        GerneralData::blueOrder[numberWarrior % 5];
    numberWarrior ++;
    switch(index){
        case 0: wari = new Iceman(wariLife[0], wariAttack[0], numberWarrior); break;
        case 1: wari = new Lion(wariLife[1], wariAttack[1], numberWarrior, 
            elements, loyalty); break;
        case 2: wari = new Wolf(wariLife[2], wariAttack[2], numberWarrior); break;
        case 3: wari = new Ninja(wariLife[3], wariAttack[3], numberWarrior); break;
        case 4: wari = new Dragon(wariLife[4], wariAttack[4], numberWarrior); break;
    }

    std::cout << std::setw(3) << std::setfill('0') << timeCurrent << ":"
        << std::setw(2) << time << std::setw(0) << " "
        << (type == RED ? "red" : "blue") << " "
        << GerneralData::wariNames[index] << " "
        << numberWarrior << " born" << std::endl;
    if(index == 1){
        std::cout << "Its loyalty is " << elements << std::endl;
    }
}

bool Headquarter::IsOccupied(){
    if(type == RED){
        return wariBlue != nullptr;
    }else{
        return wariRed != nullptr;
    }
}

Warrior* City::WariOut(int type){
    Warrior* wari;
    if(type == RED){
        wari = wariRed;
        wariRed = nullptr;
    }else{
        wari = wariBlue;
        wariBlue = nullptr;
    }
}

Warrior* Headquarter::WariOut(int type){
    Warrior* wari;
    if(type == RED){
        wari = wariRed;
        wariRed = nullptr;
    }else{
        wari = wariBlue;
        wariBlue = nullptr;
    }
}

void City::WariIn(int type, Warrior* wari, int timeCurrent, int time){
    auto& warito = type == RED ? wariRed : wariBlue;
    warito = wari;
    wari->Move();

    //000:10 red iceman 1 marched to city 1 with 20 elements and force 30
    std::cout << std::setw(3) << std::setfill('0') << timeCurrent << ":"
        << std::setw(2) << time << std::setw(0) << " "
        << (type == RED ? "red" : "blue") << " "
        << GerneralData::wariNames[index] << " "
        << wari->GetIndex() << " marched to city "
        << index << " with " << wari->GetElement()
        << " elements and force " << wari->GetForce() << std::endl;
}

void Headquarter::WariIn(int type, Warrior* wari, int timeCurrent, int time){
    auto& warito = type == RED ? wariRed : wariBlue;
    warito = wari;
    wari->Move();

    //001:10 red iceman 1 reached blue headquarter with 20 elements and force 30
    std::cout << std::setw(3) << std::setfill('0') << timeCurrent << ":"
        << std::setw(2) << time << std::setw(0) << " "
        << (type == RED ? "red" : "blue") << " "
        << GerneralData::wariNames[index] << " "
        << wari->GetIndex() << " reached "
        << (type == RED ? "red" : "blue") << " "
        << " headquarter with " << wari->GetElement()
        << " elements and force " << wari->GetForce() << std::endl;

    //003:10 blue headquarter was taken
    std::cout << std::setw(3) << std::setfill('0') << timeCurrent << ":"
        << std::setw(2) << time << std::setw(0) << " "
        << (type == RED ? "red" : "blue")
        << " headquater was taken" << std::endl;
}

void Headquarter::Report(int timeCurrent, int time){
    //000:50 100 elements in red headquarter
    std::cout << std::setw(3) << std::setfill('0') << timeCurrent << ":"
        << std::setw(2) << time << std::setw(0) << " "
        << elements << " elements in "
        << (type == RED ? "red" : "blue") << " headquarter" << std::endl;
}

void Place::WariReport(int timeCurrent, int time){
    //000:55 blue wolf 2 has 2 sword 3 bomb 0 arrow and 7 elements
    int iwr = (wariRed->GetIndex() - 1) % 5, 
        iwb = GerneralData::blueOrder[(wariBlue->GetIndex() - 1) % 5];
    int cnt[3] = {0};
    for(int i=0; i<3; ++i){
        std::vector<Weapon* >& wp = wariRed->GetWeapon();
        for(auto p : wp){
            if(p->GetType() == i){
                cnt[i] ++;
            }
        }
    }
    std::cout << std::setw(3) << std::setfill('0') << timeCurrent << ":"
        << std::setw(2) << time << std::setw(0) << " red "
        << GerneralData::wariNames[iwr] << " " << wariRed->GetIndex()
        << " has " << cnt[0] << " sword " << cnt[1] << " bomb "
        << cnt[2] << " arrow and " << wariRed->GetElement()
        << " elements" << std::endl;

    for(int i=0; i<3; ++i){
        std::vector<Weapon* >& wp = wariBlue->GetWeapon();
        for(auto p : wp){
            if(p->GetType() == i){
                cnt[i] ++;
            }
        }
    }
    std::cout << std::setw(3) << std::setfill('0') << timeCurrent << ":"
        << std::setw(2) << time << std::setw(0) << " blue "
        << GerneralData::wariNames[iwb] << " " << wariBlue->GetIndex()
        << " has " << cnt[0] << " sword " << cnt[1] << " bomb "
        << cnt[2] << " arrow and " << wariBlue->GetElement()
        << " elements" << std::endl;
}

void Place::LionRun(int timeCurrent, int time){
    //000:05 blue lion 1 ran away

    Lion* lion = dynamic_cast<Lion* >(wariRed);
    if(lion && lion.GetLoyalty() <= 0){
        std::cout << std::setw(3) << std::setfill('0') << timeCurrent << ":"
            << std::setw(2) << time << std::setw(0) << " red lion " 
            << wariRed.GetIndex() << " ran away" << std::endl;
        delete wariRed;
        wariRed = nullptr;
    }
    lion = dynamic_cast<Lion* >(wariBlue);
    if(lion && lion.GetLoyalty() <= 0){
        std::cout << std::setw(3) << std::setfill('0') << timeCurrent << ":"
            << std::setw(2) << time << std::setw(0) << " blue lion " 
            << wariBlue.GetIndex() << " ran away" << std::endl;
        delete wariBlue;
        wariBlue = nullptr;
    }
}

void City::WolfRob(int timeCurrent, int time){
    if(wariRed == nullptr || wariBlue == nullptr) return;
    Wolf* wolf = dynamic_cast<Wolf* >(wariRed);
    
    auto cmp = [](Weapon* a, Weapon* b){
        int ta = a->GetType(), tb = b->GetType();
        if(ta != tb) return ta > tb;
        if(ta == 2){
            return dynamic_cast<Arrow* >(a)->GetCount < 
                dynamic_cast<Arrow* >(b)->GetCount;
        }
    };

    //000:35 blue wolf 2 took 3 bomb from red dragon 2 in city 4
    if(wolf){
        if(!dynamic_cast<Wolf>(wariBlue)){
            std::vector<Weapon* >& wp = wariBlue->GetWeapon(), 
                & wfwp = wolf->GetWeapon();
            if(wp.size() == 0) return;

            std::sort(wp.begin(), wp.end(), cmp);
            int kd = wp.back()->GetType(), cnt = 0;
            while(wfwp.size() < 10 && wp.size() > 0 && 
                wp.back()->GetType() == kd){
                cnt ++;
                wfwp.push_back(wp.back());
                wp.pop_back();
            }
            int opo = GerneralData::blueOrder[(wariBlue->GetIndex() - 1) % 5];
            std::cout << std::setw(3) << std::setfill('0') << timeCurrent << ":"
                << std::setw(2) << time << std::setw(0) << " red wolf "
                << wolf->GetIndex() << " took " << cnt << " " 
                << GerneralData::weaponNames[kd] << " from blue "
                << GerneralData::wariNames[opo] << " " << wariBlue->GetIndex()
                << " in city " << index << std::endl;
        }
    }else{
        wolf = dynamic_cast<Wolf* >(wariBlue);
        if(wolf){
            std::vector<Weapon* >& wp = wariRed->GetWeapon(), 
                & wfwp = wolf->GetWeapon();
            if(wp.size() == 0) return;

            std::sort(wp.begin(), wp.end(), cmp);
            int kd = wp.back()->GetType(), cnt = 0;
            while(wfwp.size() < 10 && wp.size() > 0 && 
                wp.back()->GetType() == kd){
                cnt ++;
                wfwp.push_back(wp.back());
                wp.pop_back();
            }
            int opo = (wariRed->GetIndex() - 1) % 5;
            std::cout << std::setw(3) << std::setfill('0') << timeCurrent << ":"
                << std::setw(2) << time << std::setw(0) << " blue wolf "
                << wolf->GetIndex() << " took " << cnt << " " 
                << GerneralData::weaponNames[kd] << " from red "
                << GerneralData::wariNames[opo] << " " << wariRed->GetIndex()
                << " in city " << index << std::endl;
        }
    }
}

void City::Battle(int timeCurrent, int time){    
    if(wariRed == nullptr || wariBlue == nullptr) return;
    wariRed->BattleReset();
    wariBlue->BattleReset();
    //攻击，检测死亡，夺取武器，输出
    std::function<void(void)> OnceAttack = [](Warrior* atker, Warrior hurt){
        
    };
    if(index % 2 == 0){
        if(wariBlue->BattleCanAtk()) OnceAttack(wariBlue, wariRed);
    }
    while(wariBlue)
}