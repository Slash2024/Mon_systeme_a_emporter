#include "food.h"

Food::Food(const string &name, double price)
    : name(name), price(price)
{
}

string Food::getName() const
{
    return name;
}

double Food::getPrice() const
{
    return price;
}
