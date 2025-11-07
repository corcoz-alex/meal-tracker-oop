#include "Meal.h"

Meal::Meal(int id, const string& name, int calories, const string& type, const QDateTime& dateTime) :
    id(id), name(name), calories(calories), type(type), dateTime(dateTime) {}

// Getters
int Meal::getId() const{
    return id;
}

string Meal::getName() const{
    return name;
}

int Meal::getCalories() const{
    return calories;
}

string Meal::getType() const{
    return type;
}

QDateTime Meal::getDateTime() const{
    return dateTime;
}

//Setters
void Meal::setName(const string& nName){
    this->name = nName;
}

void Meal::setCalories(int nCalories){
    this->calories = nCalories;
}

void Meal::setType(const string& nType){
    this->type = nType;
}

void Meal::setDateTime(const QDateTime& nDateTime){
    this->dateTime = nDateTime;
}
