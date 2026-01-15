#include <QLayout>

#include "qinstaller.h"
#include <QDebug>

QInstaller::QInstaller(QWidget *parent)
    : QWidget{parent}
{

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    _topWidget = new QWidget(this);
    _topWidget->setObjectName("topWidget");

    _topWidget->setStyleSheet(
        "#topWidget {"
        "    border-bottom: 1px solid black;"
        "}"
        );

    _bottomWidget = new Navigator(this);

    mainLayout->addWidget(_topWidget, 8);
    mainLayout->addWidget(_bottomWidget, 2);

    _bottomWidget->setLeftButtonLabel("Back");
    _bottomWidget->setLeftButtonVisible(false);

    _bottomWidget->setRightButtonLabel("Next");

    connect(_bottomWidget, &Navigator::leftButtonClicked,
            [this]()
            {
                _pageController->prevPage();
            });
    connect(_bottomWidget, &Navigator::rightButtonClicked,
            [ this ]()
            {
                _pageController->nextPage();
            });


    _pageController = new PageController(_topWidget, this);

   connect(_pageController, &PageController::pageChanged, this, &QInstaller::handlePageChanged);
}
void QInstaller::handlePageChanged()
{
    if(_pageController->currentPage() == PageController::Page::INSTALLATION_PAGE)
    {
        _bottomWidget->setRightButtonLabel("Install");
        _bottomWidget->setRightButtonEnabled(false);
        _bottomWidget->setLeftButtonVisible(true);
    }

    if(_pageController->currentPage() == PageController::Page::WELCOME_PAGE)
    {
        _bottomWidget->setRightButtonLabel("Next");
        _bottomWidget->setRightButtonEnabled(true);
        _bottomWidget->setLeftButtonVisible(false);
    }
}
