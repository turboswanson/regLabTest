#ifndef QINSTALLER_H
#define QINSTALLER_H

#include <QWidget>
#include <QMainWindow>
#include "pageController.h"
#include "navigator.h"

class QInstaller : public QWidget
{
    Q_OBJECT
public:
    enum class Packages {NOT_SELECTED, PIP, NPM, CARGO};
    enum class EXIT_CODE {OK, ERRROR };

    explicit QInstaller(QWidget *parent = nullptr);
public slots:
    void handlePageChanged();
    void handlePipInstallFinished(const int& code);
    void handleInstallationSuccess();
    void handleInstallationFailed();
signals:
    void pipInstallFinished(const int& code);
    void installationSuccess();
    void installationFailed();
    void dialogClosed();
private:
    void handlePackageSelected(const int& selectedPackage);
    void handleInstallButtonPressed();
    void handlePipInstallation();
    void handleBusyScreen();
private:
    PageController *_topWidget;
    Navigator *_bottomWidget;
    int _selectedIndex = static_cast<int>(Packages::NOT_SELECTED);
};

#endif // QINSTALLER_H
