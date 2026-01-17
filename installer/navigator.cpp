#include "navigator.h"

#include <QDebug>
#include <QFile>
#include <QLayout>

Navigator::Navigator(QWidget* parent) : QWidget{parent}
{
    this->setMinimumHeight(100);

    QHBoxLayout* mainLayout = new QHBoxLayout(this);

    mainLayout->setContentsMargins(20, 10, 20, 10);

    mainLayout->setSpacing(0);

    QFile styleFile(":/styles/navigator.qss");
    if (styleFile.open(QFile::ReadOnly))
    {
        QString style = styleFile.readAll();
        this->setStyleSheet(style);
        ;
    }
    else
    {
        qDebug() << "Failed to load stylesheet:" << styleFile.errorString();
    }

    _leftButton = new QPushButton("");
    _leftButton->setFixedSize(120, 40);

    _rightButton = new QPushButton("");
    _rightButton->setFixedSize(120, 40);

    mainLayout->addWidget(_leftButton, 0, Qt::AlignLeft);
    mainLayout->addStretch();
    mainLayout->addWidget(_rightButton, 0, Qt::AlignRight);

    connect(_leftButton, &QPushButton::clicked, [this]() { emit leftButtonClicked(); });
    connect(_rightButton, &QPushButton::clicked, [this]() { emit rightButtonClicked(); });
}
void Navigator::setLeftButtonLabel(const QString& label)
{
    _leftButton->setText(label);
}
void Navigator::setRightButtonLabel(const QString& label)
{
    _rightButton->setText(label);
}
void Navigator::setLeftButtonVisible(const bool& visible)
{
    _leftButton->setVisible(visible);
}
void Navigator::setRightButtonVisible(const bool& visible)
{
    _rightButton->setVisible(visible);
}
void Navigator::setLeftButtonEnabled(const bool& enabled)
{
    _leftButton->setEnabled(enabled);
}
void Navigator::setRightButtonEnabled(const bool& enabled)
{
    _rightButton->setEnabled(enabled);
}

bool Navigator::isLeftButtonEnabled() const
{
    return _leftButton->isEnabled();
}

bool Navigator::isRightButtonEnabled() const
{
    return _rightButton->isEnabled();
}
