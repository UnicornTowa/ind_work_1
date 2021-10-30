#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <future>
#include <chrono>

using namespace std;

mutex lock_collection;

class Animals {
public:
    int animal_id;
    string what = "*none*";
    Animals(int _id, string _what) {
        animal_id = _id;
        what = _what;
    }
};

class Mammals : public Animals{
public:
    int num_of_legs;
    Mammals(int id, string what, int legs)
    : Animals(id, what)
    {
        num_of_legs = legs;
    }
};

class Human: public Mammals {
public:
    string gender;
    string name = "*imagine here is a unique name*";
    Human(int id, string what = "Human", int legs = 4)
    : Mammals(id, what, legs)
    {
        int i = rand() % 2;
        if (i == 0)
            gender = "Male";
        else
            gender = "Female";
    }
    void Change_Gender() {
        if (gender == "Male")
            gender = "Female";
        else if (gender == "Female")
            gender = "Male";
        else
            gender = "Helicopter";
    }
    void Plus_Leg() {
        this->num_of_legs += 1;
    }
    void Introduce() {
        cout << "Hello, I am Human №" << animal_id << ". My name is " << name << " I think my gender is " << gender << " If you are interested, I have " << num_of_legs << " legs, goodbye!" << endl;
    }
};

class Dolphin : public Mammals{
public:
    bool can_swim = true;
    Dolphin(int id, string what = "Dolphin", int legs = 0)
            : Mammals(id, what, legs)
    {}
    void Cut_Fin() {
        this->can_swim = false;
    }

    void Swim() {
        if (can_swim)
            cout << "I am swimming" << endl;
        else
            cout << "No fins :(" << endl;
    }
    void Introduce() {
        if (can_swim)
            cout << "Hello, I am dolphin №" << animal_id << " I can swim with my " << num_of_legs << "legs. Goodbye" << endl;
        else
            cout << "Hello, I am dolphin №" << animal_id << " Sorry, I can't swim, :( BTW I have " << num_of_legs << " legs. Goodbye" << endl;
    }
};

/*void Count(vector<Animals*>& collection, string what) {
    int q = 0;
    static bool ch, cd;
    if (what == "Human")
        ch = true;
    else if (what == "Dolphin")
        cd = true;
    else {
        cout << "what are u doing m8?" << endl;
        return;}
    for (Animals* i : collection) {
        auto temp = *i;
        if (i->what == what) {
            q += 1;
        }
    }
    if (q > 10) {
        if (what == "Human" && ch){
            cout << "OMG!! Congratulations! There is more than 10 " << what << "s!" << endl;
            ch = false;}
        else if (what == "Dolphin" && cd) {
            cout << "OMG!! Congratulations! There is more than 10 " << what << "s!" << endl;
            cd = false;}
        }
    cout << "There are exactly " << q << " "<< what << "s!!" << endl;
}*/

bool STOP = false;

void Count_D (vector<Animals*>& collection, int &number) {
    while (!STOP) {
        int q = 0;
        static bool once_d = true;
        lock_guard<mutex> lock(lock_collection);
        for (Animals *i: collection) {
            if (i->what == "Dolphin") {
                q += 1;
            }
        }
        number = q;
        if (number > 10 and once_d) {
            cout << "OMG there are more that 10 dolphins!!" << endl;
            once_d = false;
        }
        std::this_thread::sleep_for(250ms);
    }
}

void Count_H (vector<Animals*>& collection, int &number) {
    while (!STOP) {
        int q = 0;
        static bool once_h = true;
        lock_guard<mutex> lock(lock_collection);
        for (Animals *i: collection) {
            if (i->what == "Human") {
                q += 1;
            }
        }
        number = q;
        if (number > 10 and once_h) {
            cout << "OMG there are more that 10 dolphins!!" << endl;
            once_h = false;
        }
        std::this_thread::sleep_for(250ms);
    }
}

int id_gen() {
    static int i = 0;
    i += 1;
    return i;
}

int main() {
    Human a(id_gen());
    a.gender = "Male";
    a.Change_Gender();
    a.Introduce();
    Dolphin d(id_gen());
    d.Cut_Fin();
    d.Introduce();
    cout << a.gender << endl;
    Human h1(id_gen());
    Human h2(id_gen());
    Human h3(id_gen());
    Human h4(id_gen());
    Human h5(id_gen());
    Dolphin d1(id_gen());
    Dolphin d2(id_gen());
    Dolphin d3(id_gen());
    Dolphin d4(id_gen());
    Dolphin d5(id_gen());


    vector<Animals*> collection = {
            &h1, &h2, &h3, &h4, &h5, &d1, &d2, &d3, &d4, &d5
    };
    int n_of_dolphins = 0, n_of_humans = 0;
    thread count_dolphins_parallel(Count_D, ref(collection), ref(n_of_dolphins));
    thread count_humans_parallel(Count_H, ref(collection), ref(n_of_humans));

    vector<Dolphin> dolphin_storage;
    vector<Human> human_storage;
    cout << "Сколько добавляем?" << endl;
    int quantity;
    cin >> quantity;
    while (quantity > 0) {
        char type;
        cout << "Human or Dolphin? H/D" << endl;
        cin >> type;
        if (type == 'H') {
            int id, legs;
            string name, gender;
            cout << "Введите последовательно ID, количество ног, имя и гендер человека" << endl;
            cin >> id >> legs >> name >> gender;
            Human temp(id);
            temp.gender = gender;
            temp.name = name;
            temp.num_of_legs = legs;
            human_storage.push_back(temp);
            auto *temp_ptr = &human_storage.back();
            collection.push_back(temp_ptr);
        }
        else if (type == 'D') {
            int id, legs, swim;
            bool swim_bool;
            cout << "Введите последовательно ID, количество ног y дельфина. Напишите 1/0 в зависимости от того может ли он плавать" << endl;
            cin >> id >> legs >> swim;
            if (swim == 0)
                swim_bool = false;
            else if (swim == 1)
                swim_bool = true;
            else {
                cout << "Я же попросил, 0 или 1" << endl;
                break;
            }
            Dolphin temp(id);
            temp.can_swim = swim_bool;
            temp.num_of_legs = legs;
            dolphin_storage.push_back(temp);
            auto *temp_ptr = &dolphin_storage.back();
            collection.push_back(temp_ptr);
        }
        else {
            cout << "Error, shutting down" << endl;
            break;
        }
        quantity -= 1;
    }
    //thread count_humans(Count, ref(collection), "Human");
    //thread count_dolphins(Count, ref(collection), "Dolphin");
    //count_humans.join();
    //count_dolphins.join();

    count_dolphins_parallel.join();
    count_humans_parallel.join();
    STOP = true;
    cout << STOP << endl;
    cout << "There are exactly " << n_of_dolphins << " dolphins and " << n_of_humans << " humans" << endl;
