#ifndef PAGECONTROLLER_H
#define PAGECONTROLLER_H

#include <QComboBox>
#include <QObject>
#include <QStackedWidget>

class PageController : public QWidget
{
    Q_OBJECT
public:
    enum class Page
    {
        WELCOME_PAGE,
        INSTALLATION_PAGE
    };

    explicit PageController(QWidget* parent = nullptr);
    void setCurrentPageIndex(int newCurrentPageIndex);
    Page currentPage() const;
    void nextPage();
    void prevPage();
    void setCurrentPackageIndex(const int& index);
signals:
    void pageChanged();
    void packageSelected(const int& packageName);

private:
    void setUpWelcomePage(const QString& welcomeText);
    void setUpInstallationPage();

private:
    QStackedWidget* _stackedWidget;
    QWidget* _welcomePage;
    QWidget* _installationPage;
    QComboBox* _packagesList;
    int _currentPageIndex = 0;
};

#endif // PAGECONTROLLER_H
