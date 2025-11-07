#include <gtest/gtest.h>
#include <QDateTime>
#include "../domain/Meal.h"
#include "../repository/CSVRepository.h"
#include "../repository/jsonrepository.h"
#include "../controller/Controller.h"
#include "../filter/filterbyname.h"
#include "../filter/filterbytype.h"
#include "../filter/filterbycaloriesrange.h"
#include "../filter/andfilter.h"
#include "../filter/orfilter.h"

// --------- Meal Class Tests ---------
TEST(MealTest, GettersSetters) {
    QDateTime dt = QDateTime::currentDateTime();
    Meal m(1, std::string("Pizza"), 500, std::string("Dinner"), dt);
    EXPECT_EQ(m.getId(), 1);
    EXPECT_EQ(m.getName(), std::string("Pizza"));
    EXPECT_EQ(m.getCalories(), 500);
    EXPECT_EQ(m.getType(), std::string("Dinner"));
    EXPECT_EQ(m.getDateTime(), dt);
}

// --------- CSVRepository Tests ---------
TEST(CSVRepositoryTest, AddRemoveUpdate) {
    CSVRepository repo("test.csv");
    QDateTime dt = QDateTime::currentDateTime();
    Meal m(1, std::string("Apple"), 50, std::string("Snack"), dt);

    repo.addMeal(m);
    EXPECT_EQ(repo.getAllMeals().size(), 1);

    repo.removeMeal(1);
    EXPECT_EQ(repo.getAllMeals().size(), 0);

    repo.addMeal(m);
    Meal m2(1, std::string("Banana"), 100, std::string("Breakfast"), dt);
    repo.updateMeal(m2);
    EXPECT_EQ(repo.getAllMeals()[0].getName(), std::string("Banana"));

    // Clean up
    remove("test.csv");
}

// --------- JSONRepository Tests ---------
TEST(JSONRepositoryTest, SaveLoad) {
    JSONRepository repo("test.json");
    QDateTime dt = QDateTime::currentDateTime();
    Meal m(1, std::string("Orange"), 80, std::string("Snack"), dt);
    repo.addMeal(m);
    repo.save();

    JSONRepository repo2("test.json");
    repo2.load();
    auto meals = repo2.getAllMeals();
    ASSERT_EQ(meals.size(), 1);
    EXPECT_EQ(meals[0].getName(), std::string("Orange"));

    // Clean up
    remove("test.json");
}

// --------- Controller Tests ---------
TEST(ControllerTest, AddRemoveUpdateUndoRedo) {
    auto repo = std::make_unique<CSVRepository>("test2.csv");
    Controller ctrl(std::move(repo));
    QDateTime dt = QDateTime::currentDateTime();

    ctrl.addMeal("Banana", 100, "Breakfast", dt);
    EXPECT_EQ(ctrl.getAllMeals().size(), 1);

    int id = ctrl.getAllMeals()[0].getId();
    ctrl.removeMeal(id);
    EXPECT_EQ(ctrl.getAllMeals().size(), 0);

    ctrl.undo();
    EXPECT_EQ(ctrl.getAllMeals().size(), 1);

    ctrl.redo();
    EXPECT_EQ(ctrl.getAllMeals().size(), 0);

    ctrl.addMeal("Eggs", 200, "Breakfast", dt);
    ctrl.updateMeal(ctrl.getAllMeals()[0].getId(), "Eggs Benedict", 250, "Breakfast", dt);
    EXPECT_EQ(ctrl.getAllMeals()[0].getName(), std::string("Eggs Benedict"));

    // Clean up
    remove("test2.csv");
}

TEST(ControllerTest, Filtering) {
    auto repo = std::make_unique<CSVRepository>("test3.csv");
    Controller ctrl(std::move(repo));
    QDateTime dt = QDateTime::currentDateTime();

    ctrl.addMeal("Apple", 50, "Snack", dt);
    ctrl.addMeal("Banana", 100, "Breakfast", dt);
    ctrl.addMeal("Pizza", 700, "Dinner", dt);

    // Filter by name
    FilterByName nameFilter("Apple");
    auto result = ctrl.filterMeals(nameFilter);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].getName(), std::string("Apple"));

    // Filter by type
    FilterByType typeFilter("Breakfast");
    result = ctrl.filterMeals(typeFilter);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].getName(), std::string("Banana"));

    // Filter by calories range
    FilterByCaloriesRange calFilter(60, 800);
    result = ctrl.filterMeals(calFilter);
    ASSERT_EQ(result.size(), 2);

    // AND filter
    std::vector<std::shared_ptr<FilterStrategy>> filters;
    filters.push_back(std::make_shared<FilterByType>("Snack"));
    filters.push_back(std::make_shared<FilterByCaloriesRange>(40, 60));
    AndFilter andFilter(filters);
    result = ctrl.filterMeals(andFilter);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].getName(), std::string("Apple"));

    // OR filter
    filters.clear();
    filters.push_back(std::make_shared<FilterByType>("Snack"));
    filters.push_back(std::make_shared<FilterByType>("Dinner"));
    OrFilter orFilter(filters);
    result = ctrl.filterMeals(orFilter);
    ASSERT_EQ(result.size(), 2);

    // Clean up
    remove("test3.csv");
}

// --------- Additional Tests ---------

// Test Meal setters and getters
TEST(MealTest, SettersAndGetters) {
    QDateTime dt = QDateTime::currentDateTime();
    Meal m(1, std::string("Test"), 100, std::string("Type"), dt);

    m.setName("NewName");
    m.setCalories(200);
    m.setType("NewType");
    QDateTime newDt = dt.addDays(1);
    m.setDateTime(newDt);

    EXPECT_EQ(m.getName(), std::string("NewName"));
    EXPECT_EQ(m.getCalories(), 200);
    EXPECT_EQ(m.getType(), std::string("NewType"));
    EXPECT_EQ(m.getDateTime(), newDt);
}

// Test repository getMealById and edge case for not found
TEST(CSVRepositoryTest, GetMealById) {
    CSVRepository repo("test_getbyid.csv");
    QDateTime dt = QDateTime::currentDateTime();
    Meal m(42, std::string("FindMe"), 123, std::string("Lunch"), dt);
    repo.addMeal(m);

    Meal found = repo.getMealById(42);
    EXPECT_EQ(found.getName(), std::string("FindMe"));

    // Clean up
    remove("test_getbyid.csv");
}

// Test undo/redo for clear operation
TEST(ControllerTest, UndoRedoClear) {
    auto repo = std::make_unique<CSVRepository>("test_clear.csv");
    Controller ctrl(std::move(repo));
    QDateTime dt = QDateTime::currentDateTime();

    ctrl.addMeal("A", 1, "T", dt);
    ctrl.addMeal("B", 2, "T", dt);
    EXPECT_EQ(ctrl.getAllMeals().size(), 2);

    ctrl.clearAllMealsWithUndo();
    EXPECT_EQ(ctrl.getAllMeals().size(), 0);

    ctrl.undo();
    EXPECT_EQ(ctrl.getAllMeals().size(), 2);

    ctrl.redo();
    EXPECT_EQ(ctrl.getAllMeals().size(), 0);

    // Clean up
    remove("test_clear.csv");
}

// Test updateMeal actually updates all fields
TEST(ControllerTest, UpdateMealAllFields) {
    auto repo = std::make_unique<CSVRepository>("test_update.csv");
    Controller ctrl(std::move(repo));
    QDateTime dt = QDateTime::currentDateTime();

    ctrl.addMeal("Old", 10, "Type1", dt);
    int id = ctrl.getAllMeals()[0].getId();
    QDateTime newDt = dt.addDays(2);
    ctrl.updateMeal(id, "New", 99, "Type2", newDt);

    Meal updated = ctrl.getAllMeals()[0];
    EXPECT_EQ(updated.getName(), std::string("New"));
    EXPECT_EQ(updated.getCalories(), 99);
    EXPECT_EQ(updated.getType(), std::string("Type2"));
    EXPECT_EQ(updated.getDateTime(), newDt);

    // Clean up
    remove("test_update.csv");
}

// --------- Main ---------
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    if (result == 0) {
        std::cout << "\nAll tests passed successfully!" << std::endl;
    }
    return result;
}
