#ifndef QINSTALLER_H
#define QINSTALLER_H

#include "navigator.h"
#include "pageController.h"

#include <QMainWindow>
#include <QWidget>

class QInstaller : public QWidget
{
    Q_OBJECT
public:
    enum class Packages
    {
        NOT_SELECTED,
        PIP,
        NPM
    };
    enum class EXIT_CODE
    {
        OK,
        ERROR
    };

    explicit QInstaller(QWidget* parent = nullptr);
public slots:
    void handlePageChanged();
    void handlePackageSelected(const int& selectedPackage);
signals:
    void installationFinished(const int& code);

private:
    void handleInstallButtonPressed();
    void handleInstallation(const Packages& selectedPackage);
    void handleBusyScreen();
    QString packageToString(const Packages& pkg);

private:
    PageController* _topWidget;
    Navigator* _bottomWidget;
    int _selectedIndex = static_cast<int>(Packages::NOT_SELECTED);
};

#endif // QINSTALLER_H
