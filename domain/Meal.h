#ifndef MEAL_H
#define MEAL_H
#include <string>
#include <QDateTime>

using std::string;

class Meal
{
private:
    int id;
    string name;
    int calories;
    string type;
    QDateTime dateTime;
public:
    Meal(int id, const string& name, int calories, const string& type, const QDateTime& dateTime);
    // Getter methods
    int getId() const;
    string getName() const;
    int getCalories() const;
    string getType() const;
    QDateTime getDateTime() const;

    // Setter methods
    void setName(const string& name);
    void setCalories(int calories);
    void setType(const string& type);
    void setDateTime(const QDateTime& dateTime);
};

#endif // MEAL_H
