#include "mainwindow.h"
#include <QLayout>
#include "navigator.h"

MainWindow::MainWindow(QWidget* parent)
: QMainWindow(parent)
{
    setWindowTitle("Package installer");
    resize(1200, 768);

    QWidget* mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);

    _installer = new QInstaller(this);

    mainLayout->addWidget(_installer);

}
MainWindow::~MainWindow()
{}


