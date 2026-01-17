#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include <QPushButton>
#include <QWidget>

class Navigator : public QWidget
{
    Q_OBJECT
public:
    explicit Navigator(QWidget* parent = nullptr);
    void setLeftButtonLabel(const QString& label);
    void setRightButtonLabel(const QString& label);
    void setLeftButtonVisible(const bool& visible);
    void setRightButtonVisible(const bool& visible);
    void setLeftButtonEnabled(const bool& enabled);
    void setRightButtonEnabled(const bool& enabled);
    bool isLeftButtonEnabled() const;
    bool isRightButtonEnabled() const;

signals:
    void leftButtonClicked();
    void rightButtonClicked();

private:
    QPushButton* _leftButton;
    QPushButton* _rightButton;
};

#endif // NAVIGATOR_H
