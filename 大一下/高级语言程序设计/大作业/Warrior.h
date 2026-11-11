class Weapon{
    protected:
    int type;
    bool status;

    public:
    virtual int Damage(int) = 0;
    int GetType() const { return type; }
    bool Exist() const { return status; }
};

class Sword : public Weapon{
    public:
    Sword() : type(0), status(1) {}
    int Damage(int);
};

class Bomb : public Weapon{
    public:
    Bomb() : type(1), status(1) {}
    int Damage(int);
};

class Arrow : public Weapon{
    private:
    int count;

    public:
    Arrow() : type(2), status(1), count(2) {}
    int GetCount() const { return count; }
    int Damage(int);
};

class Warrior{
    protected:
    int elements, force, index;
    std::vector<Weapon *> weapon;
    Weapon* InitialWeapon(int);

    public:
    Warrior(int, int, int);
    void BattleReset();
    int BattleAttack();
    void BattleHurt(int);
    bool BattleCanAtk() { return weapon.size(); };

    virtual void Move(){}

    std::vector<Weapon *>& GetWeapon(){ return weapon; }
    int GetElement(){ return elements; }
    int GetForce(){ return force; }
    int GetIndex(){ return index; }
};

class Iceman : public Warrior{
    public:
    Iceman(int, int, int);
    void Move();
};

class Lion : public Warrior{
    protected:
    int loyalty;
    int decreace;
    
    public:
    Lion(int, int, int, int);
    void Move();
    int GetLoyalty(){ return loyalty; }
};

class Wolf : public Warrior{
    public:
    Wolf(int, int, int);
};

class Ninja : public Warrior{
    public:
    Ninja(int, int, int);
};

class Dragon : public Warrior{
    public:
    Dragon(int, int, int);
}

int Sword::Damage(int force){
    return force * 0.2;
}

int Bomb::Damage(int force){
    status = 0;
    return force * 0.4;
}

int Arrow::Damage(int force){
    count --;
    if(count <= 0) status = 0;
    return force * 0.3;
}

Warrior::Warrior(int life, int force, int index)
    : elements(life), force(force), index(index){
}

Weapon* Warrior::InitialWeapon(int index){
    switch(index){
        case 0: return new Sword();
        case 1: return new Bomb();
        case 2: return new Arrow();
    }
}

Iceman::Iceman(int life, int force, int index)
    : Warrior(life, force, index){
    weapon.push_back(InitialWeapon(index % 3));
}

Lion::Lion(int life, int force, int index, int loyalty, int decreace)
    : Warrior(life, force, index), loyalty(loyalty), decreace(decreace){
    weapon.push_back(InitialWeapon(index % 3));
}

Wolf::Wolf(int life, int force, int index)
    : Warrior(life, force, index){
}

Ninja::Ninja(int life, int force, int index)
    : Warrior(life, force, index){
    weapon.push_back(InitialWeapon(index % 3));
    weapon.push_back(InitialWeapon((index + 1) % 3));
}

Dragon::Dragon(int life, int force, int index)
    : Warrior(life, force, index){
    weapon.push_back(InitialWeapon(index % 3));
}

void Iceman::Move(){
    int dmg = elements * 0.1;
    elements -= dmg;
}

void Lion::Move(){
    loyalty -= decreace;
}