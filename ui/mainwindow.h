#pragma once

#include <QMainWindow>
#include "../controller/controller.h"
#include "../repository/jsonrepository.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(Controller& controller, QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Controller& controller;
    JSONRepository repository{"meals.json"};

    void connectSignals();
    void refreshMealTable();
    void clearForm();
    void fillFormFromSelectedRow();
    void saveData();

private slots:
    void onAddMeal();
    void onClearForm();
    void onRemoveMeal();
    void onUpdateMeal();
    void onUndo();
    void onRedo();
    void onApplyFilter();
    void onClearFilter();
    void onNameFilterToggled(bool checked);
    void onTypeFilterToggled(bool checked);
    void onCaloriesFilterToggled(bool checked);
};
