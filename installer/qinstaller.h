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
    explicit QInstaller(QWidget *parent = nullptr);
public slots:
    void handlePageChanged();
signals:
private:
    QWidget *_topWidget;
    Navigator *_bottomWidget;
    PageController *_pageController;
};

#endif // QINSTALLER_H
