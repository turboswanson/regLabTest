#ifndef PAGECONTROLLER_H
#define PAGECONTROLLER_H

#include <QObject>
#include <QStackedWidget>

class PageController : public QObject
{
    Q_OBJECT
public:
    enum class Page {
        WELCOME_PAGE,
        INSTALLATION_PAGE
    };

    explicit PageController(QWidget *containerWidget, QObject *parent = nullptr);
    void setCurrentPageIndex(int newCurrentPageIndex);
    Page currentPage() const;
    void nextPage();
    void prevPage();
signals:
    void pageChanged();
private:
    void setUpWelcomePage();
    void setUpInstallationPage();
private:
    QStackedWidget *_stackedWidget;
    QWidget *_welcomePage;
    QWidget *_installationPage;
    int _currentPageIndex = 0;
};

#endif // PAGECONTROLLER_H
