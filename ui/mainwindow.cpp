#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "../filter/filterbyname.h"
#include "../filter/filterbytype.h"
#include "../filter/filterbycaloriesrange.h"
#include "../filter/andfilter.h"
#include "../filter/orfilter.h"

MainWindow::MainWindow(Controller& controller, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), controller(controller), repository("meals.json")
{
    ui->setupUi(this);
    ui->filterGroupBox->setVisible(false);  // Start hidden
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    
    // Make table read-only
    ui->mealTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    // Load saved data
    repository.load();
    auto meals = repository.getAllMeals();
    for (const auto& meal : meals) {
        controller.addMeal(meal.getName(), meal.getCalories(), meal.getType(), meal.getDateTime());
    }
    
    connectSignals();
    refreshMealTable();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::connectSignals() {
    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::onAddMeal);
    connect(ui->clearButton, &QPushButton::clicked, this, &MainWindow::onClearForm);
    connect(ui->removeButton, &QPushButton::clicked, this, &MainWindow::onRemoveMeal);
    connect(ui->updateButton, &QPushButton::clicked, this, &MainWindow::onUpdateMeal);
    connect(ui->undoButton, &QPushButton::clicked, this, &MainWindow::onUndo);
    connect(ui->redoButton, &QPushButton::clicked, this, &MainWindow::onRedo);
    connect(ui->mealTable, &QTableWidget::cellClicked, this, &MainWindow::fillFormFromSelectedRow);
    connect(ui->applyFilterBtn, &QPushButton::clicked, this, &MainWindow::onApplyFilter);
    connect(ui->clearFilterBtn, &QPushButton::clicked, this, &MainWindow::onClearFilter);
    connect(ui->filterToggleButton, &QPushButton::clicked, [this]() {
        bool visible = ui->filterGroupBox->isVisible();
        ui->filterGroupBox->setVisible(!visible);
    });
    
    // Connect checkbox signals
    connect(ui->filterNameCheckBox, &QCheckBox::toggled, this, &MainWindow::onNameFilterToggled);
    connect(ui->filterTypeCheckBox, &QCheckBox::toggled, this, &MainWindow::onTypeFilterToggled);
    connect(ui->filterCaloriesCheckBox, &QCheckBox::toggled, this, &MainWindow::onCaloriesFilterToggled);
}

void MainWindow::refreshMealTable() {
    auto meals = controller.getAllMeals();
    ui->mealTable->setRowCount(static_cast<int>(meals.size()));
    ui->mealTable->setColumnCount(5);
    QStringList headers = {"ID", "Name", "Calories", "Type", "Date/Time"};
    ui->mealTable->setHorizontalHeaderLabels(headers);

    // Set the table to stretch horizontally to fill available space
    ui->mealTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    
    // Set vertical header to resize to contents
    ui->mealTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    for (int i = 0; i < meals.size(); ++i) {
        const Meal& m = meals[i];
        ui->mealTable->setItem(i, 0, new QTableWidgetItem(QString::number(m.getId())));
        ui->mealTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(m.getName())));
        ui->mealTable->setItem(i, 2, new QTableWidgetItem(QString::number(m.getCalories())));
        ui->mealTable->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(m.getType())));
        ui->mealTable->setItem(i, 4, new QTableWidgetItem(m.getDateTime().toString(Qt::ISODate)));
    }
}

void MainWindow::clearForm() {
    ui->nameInput->clear();
    ui->calorieInput->setValue(0);
    ui->typeComboBox->setCurrentIndex(0);
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
}

void MainWindow::saveData() {
    // Clear the repository first
    auto meals = controller.getAllMeals();
    repository = JSONRepository("meals.json");  // Create a fresh repository
    
    // Add all current meals
    for (const auto& meal : meals) {
        repository.addMeal(meal);
    }
    repository.save();
}

void MainWindow::onAddMeal() {
    QString name = ui->nameInput->text();
    int calories = ui->calorieInput->value();
    QString type = ui->typeComboBox->currentText();
    QDateTime dt = ui->dateTimeEdit->dateTime();

    if (name.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Meal name cannot be empty.");
        return;
    }

    controller.addMeal(name.toStdString(), calories, type.toStdString(), dt);
    refreshMealTable();
    clearForm();
    saveData();
}

void MainWindow::onClearForm() {
    controller.clearAllMealsWithUndo();
    // Clear the form inputs
    ui->nameInput->clear();
    ui->calorieInput->setValue(0);
    ui->typeComboBox->setCurrentIndex(0);
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    // Refresh the table to show empty state
    refreshMealTable();
    saveData();
}

void MainWindow::onRemoveMeal() {
    auto items = ui->mealTable->selectedItems();
    if (items.isEmpty()) return;

    int row = items.first()->row();
    int id = ui->mealTable->item(row, 0)->text().toInt();
    controller.removeMeal(id);
    refreshMealTable();
    saveData();
}

void MainWindow::onUpdateMeal() {
    auto items = ui->mealTable->selectedItems();
    if (items.isEmpty()) return;

    int row = items.first()->row();
    int id = ui->mealTable->item(row, 0)->text().toInt();

    QString name = ui->nameInput->text();
    int calories = ui->calorieInput->value();
    QString type = ui->typeComboBox->currentText();
    QDateTime dt = ui->dateTimeEdit->dateTime();

    controller.updateMeal(id, name.toStdString(), calories, type.toStdString(), dt);
    refreshMealTable();
    clearForm();
    saveData();
}

void MainWindow::onUndo() {
    controller.undo();
    refreshMealTable();
}

void MainWindow::onRedo() {
    controller.redo();
    refreshMealTable();
}

void MainWindow::fillFormFromSelectedRow() {
    auto items = ui->mealTable->selectedItems();
    if (items.isEmpty()) return;

    int row = items.first()->row();
    ui->nameInput->setText(ui->mealTable->item(row, 1)->text());
    ui->calorieInput->setValue(ui->mealTable->item(row, 2)->text().toInt());
    ui->typeComboBox->setCurrentText(ui->mealTable->item(row, 3)->text());
    ui->dateTimeEdit->setDateTime(QDateTime::fromString(ui->mealTable->item(row, 4)->text(), Qt::ISODate));
}

void MainWindow::onNameFilterToggled(bool checked) {
    ui->filterNameInput->setEnabled(checked);
}

void MainWindow::onTypeFilterToggled(bool checked) {
    ui->filterTypeCombo->setEnabled(checked);
}

void MainWindow::onCaloriesFilterToggled(bool checked) {
    ui->minCalInput->setEnabled(checked);
    ui->maxCalInput->setEnabled(checked);
}

void MainWindow::onApplyFilter() {
    std::vector<std::shared_ptr<FilterStrategy>> filters;

    // Only add name filter if checkbox is checked and input is not empty
    if (ui->filterNameCheckBox->isChecked()) {
        QString name = ui->filterNameInput->text();
        if (!name.isEmpty()) {
            filters.push_back(std::make_shared<FilterByName>(name.toStdString()));
        }
    }

    // Only add type filter if checkbox is checked and type is not "Any"
    if (ui->filterTypeCheckBox->isChecked()) {
        QString type = ui->filterTypeCombo->currentText();
        if (type != "Any") {
            filters.push_back(std::make_shared<FilterByType>(type.toStdString()));
        }
    }

    // Only add calories filter if checkbox is checked and range is valid
    if (ui->filterCaloriesCheckBox->isChecked()) {
        int minCal = ui->minCalInput->value();
        int maxCal = ui->maxCalInput->value();
        if (minCal <= maxCal) {
            filters.push_back(std::make_shared<FilterByCaloriesRange>(minCal, maxCal));
        }
    }

    std::shared_ptr<FilterStrategy> combinedFilter;

    if (filters.empty()) {
        refreshMealTable();
        return;
    } else if (filters.size() == 1) {
        combinedFilter = filters[0];
    } else {
        QString logic = ui->filterLogicCombo->currentText();
        if (logic == "AND") {
            combinedFilter = std::make_shared<AndFilter>(filters);
        } else {
            combinedFilter = std::make_shared<OrFilter>(filters);
        }
    }

    auto result = controller.filterMeals(*combinedFilter);

    // Show filtered results
    ui->mealTable->setRowCount(static_cast<int>(result.size()));
    for (int i = 0; i < result.size(); ++i) {
        const Meal& m = result[i];
        ui->mealTable->setItem(i, 0, new QTableWidgetItem(QString::number(m.getId())));
        ui->mealTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(m.getName())));
        ui->mealTable->setItem(i, 2, new QTableWidgetItem(QString::number(m.getCalories())));
        ui->mealTable->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(m.getType())));
        ui->mealTable->setItem(i, 4, new QTableWidgetItem(m.getDateTime().toString(Qt::ISODate)));
    }
    // Ensure columns stretch to fill the table width after filtering
    ui->mealTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::onClearFilter() {
    // Uncheck all checkboxes
    ui->filterNameCheckBox->setChecked(false);
    ui->filterTypeCheckBox->setChecked(false);
    ui->filterCaloriesCheckBox->setChecked(false);
    
    // Clear all inputs
    ui->filterNameInput->clear();
    ui->filterTypeCombo->setCurrentIndex(0); // "Any"
    ui->minCalInput->setValue(0);
    ui->maxCalInput->setValue(2000);
    
    refreshMealTable();
}

