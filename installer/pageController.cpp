#include "pageController.h"
#include <QLabel>
#include <QLayout>
#include <QDebug>

PageController::PageController(QWidget* containerWidget, QObject* parent)
{
    _stackedWidget = new QStackedWidget(containerWidget);
    QVBoxLayout* containerLayout = new QVBoxLayout(containerWidget);
    containerLayout->addWidget(_stackedWidget);
    setUpWelcomePage();
    setUpInstallationPage();
    _stackedWidget->addWidget(_welcomePage);
    _stackedWidget->addWidget(_installationPage);
    _stackedWidget->setCurrentIndex(_currentPageIndex);
}
void PageController::setUpWelcomePage()
{
    _welcomePage = new QWidget();
    QGridLayout *welcomeLayout = new QGridLayout(_welcomePage);
    QLabel *welcomeLabel = new QLabel("Welcome");
    welcomeLabel->setAlignment(Qt::AlignCenter);
    welcomeLabel->setFont((QFont("Arial", 28, QFont::Bold)));
    welcomeLabel->setStyleSheet("color: red; border: 2px solid blue; border-radius: 10px; padding: 15px;");
    welcomeLayout->addWidget(welcomeLabel, 0, 0, Qt::AlignCenter);
    welcomeLayout->setRowStretch(0, 1);
    welcomeLayout->setColumnStretch(0, 1);
}
void PageController::setUpInstallationPage()
{
    _installationPage = new QWidget();
    QVBoxLayout *installationLayout = new QVBoxLayout(_installationPage);
    QLabel *installationLabel = new QLabel("Installation");
    installationLabel->setAlignment(Qt::AlignCenter);
    installationLabel->setFont((QFont("Arial", 24)));
    installationLayout->addWidget(installationLabel);
}
void PageController::setCurrentPageIndex(int newCurrentPageIndex)
{
    _currentPageIndex = newCurrentPageIndex;
}
void PageController::nextPage()
{
    if(_currentPageIndex < _stackedWidget->count() - 1)
    {
        _currentPageIndex++;
        _stackedWidget->setCurrentIndex(_currentPageIndex);

        emit pageChanged();

    }
}

void PageController::prevPage()
{
    if(_currentPageIndex)
    {
        _currentPageIndex--;
        _stackedWidget->setCurrentIndex(_currentPageIndex);

        emit pageChanged();
    }
}

PageController::Page PageController::currentPage() const
{
    return static_cast<Page>(_currentPageIndex);
}
