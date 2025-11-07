#include "CSVRepository.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

CSVRepository::CSVRepository(const std::string& filename)
    : filename(filename) {}

void CSVRepository::addMeal(const Meal& meal) {
    meals.push_back(meal);
}

void CSVRepository::removeMeal(int id) {
    meals.erase(
        std::remove_if(meals.begin(), meals.end(),
                       [id](const Meal& m) {
                           return m.getId() == id;
                       }),
        meals.end()
        );
}

void CSVRepository::updateMeal(const Meal& meal) {
    for (auto& m : meals) {
        if (m.getId() == meal.getId()) {
            m = meal;
            return;
        }
    }
    throw std::runtime_error("Meal not found for update.");
}

std::vector<Meal> CSVRepository::getAllMeals() const {
    return meals;
}

Meal CSVRepository::getMealById(int id) const {
    for (const auto& m : meals) {
        if (m.getId() == id) {
            return m;
        }
    }
    throw std::runtime_error("Meal not found.");
}

void CSVRepository::load() {
    meals.clear();
    std::ifstream in(filename);
    if (!in.is_open()) return;

    std::string line;
    while (std::getline(in, line)) {
        std::stringstream ss(line);
        std::string idStr, name, caloriesStr, type, datetimeStr;

        std::getline(ss, idStr, ',');
        std::getline(ss, name, ',');
        std::getline(ss, caloriesStr, ',');
        std::getline(ss, type, ',');
        std::getline(ss, datetimeStr, ',');

        int id = std::stoi(idStr);
        int calories = std::stoi(caloriesStr);
        QDateTime dt = QDateTime::fromString(QString::fromStdString(datetimeStr), Qt::ISODate);

        meals.emplace_back(id, name, calories, type, dt);
    }

    in.close();
}

void CSVRepository::save() const {
    std::ofstream out(filename);
    for (const auto& m : meals) {
        out << m.getId() << ","
            << m.getName() << ","
            << m.getCalories() << ","
            << m.getType() << ","
            << m.getDateTime().toString(Qt::ISODate).toStdString()
            << "\n";
    }
    out.close();
}
