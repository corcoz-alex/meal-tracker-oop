#include "jsonrepository.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

JSONRepository::JSONRepository(const std::string& filename)
    : filename(filename) {}

void JSONRepository::addMeal(const Meal& meal) {
    meals.push_back(meal);
}

void JSONRepository::removeMeal(int id) {
    meals.erase(
        std::remove_if(meals.begin(), meals.end(),
                       [id](const Meal& m) { return m.getId() == id; }),
        meals.end());
}

void JSONRepository::updateMeal(const Meal& meal) {
    for (auto& m : meals) {
        if (m.getId() == meal.getId()) {
            m = meal;
            return;
        }
    }
}

std::vector<Meal> JSONRepository::getAllMeals() const {
    return meals;
}

Meal JSONRepository::getMealById(int id) const {
    for (const auto& m : meals)
        if (m.getId() == id)
            return m;
    throw std::runtime_error("Meal not found.");
}

void JSONRepository::load() {
    meals.clear();

    QFile file(QString::fromStdString(filename));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QByteArray raw = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(raw);
    QJsonArray array = doc.array();

    for (const auto& val : array) {
        QJsonObject obj = val.toObject();
        Meal m(
            obj["id"].toInt(),
            obj["name"].toString().toStdString(),
            obj["calories"].toInt(),
            obj["type"].toString().toStdString(),
            QDateTime::fromString(obj["datetime"].toString(), Qt::ISODate)
            );
        meals.push_back(m);
    }
    file.close();
}

void JSONRepository::save() const {
    QJsonArray array;
    for (const auto& m : meals) {
        QJsonObject obj;
        obj["id"] = m.getId();
        obj["name"] = QString::fromStdString(m.getName());
        obj["calories"] = m.getCalories();
        obj["type"] = QString::fromStdString(m.getType());
        obj["datetime"] = m.getDateTime().toString(Qt::ISODate);
        array.append(obj);
    }

    QJsonDocument doc(array);
    QFile file(QString::fromStdString(filename));
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.write(doc.toJson());
        file.close();
    }
}
