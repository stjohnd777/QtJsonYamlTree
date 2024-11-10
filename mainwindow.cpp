#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "./jsontree.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    std::string jsonPath = "/Users/danielst.johnst.john/development/QT/JsonTree/data.json";
    JsonTree *widget = new JsonTree(jsonPath,this);
    ui->verticalLayout->addWidget(widget);

}

MainWindow::~MainWindow()
{
    delete ui;
}
