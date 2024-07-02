//菜品类

#ifndef FOOD_H
#define FOOD_H

#include <string>

using namespace std;

class Food
{
public:
    Food(const string &name = "", double price = 0.0);
    string getName() const;
    double getPrice() const;

private:
    string name;
    double price;
};

#endif // FOOD_H
