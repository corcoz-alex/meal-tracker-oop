#include <QApplication>
#include <QDir>
#include "ui/mainwindow.h"
#include "controller/controller.h"
#include "repository/jsonrepository.h"  // or CSVRepository if preferred

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Define a path to the storage file (relative to the executable)
    QString basePath = QDir::currentPath();
    QString pathToJson = basePath + "/../meals.json";  // or .csv

    // Initialize repository (JSON or CSV)
    std::unique_ptr<IRepository> repo = std::make_unique<JSONRepository>(pathToJson.toStdString());

    // Create controller
    Controller controller(std::move(repo));
    controller.load();

    // Launch UI
    MainWindow window(controller);
    window.show();

    return app.exec();
}
