#include <QLayout>
#include <QProcess>
#include "qinstaller.h"
#include <QDebug>
#include <QDir>
#include <QDialog>
#include <QLabel>

QInstaller::QInstaller(QWidget *parent)
    : QWidget{parent}
{

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    _topWidget = new PageController(this);
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
                _topWidget->prevPage();
            });
    connect(_bottomWidget, &Navigator::rightButtonClicked,
            [ this ]()
            {
                if(_topWidget->currentPage() == PageController::Page::WELCOME_PAGE)
                {
                    _topWidget->nextPage();
                } else if(_topWidget->currentPage() == PageController::Page::INSTALLATION_PAGE)
                {
                    handleInstallButtonPressed();
                }
            });

   connect(_topWidget, &PageController::pageChanged, this, &QInstaller::handlePageChanged);
   connect(_topWidget, &PageController::packageSelected, [this](const int& selectedIndex) {handlePackageSelected(selectedIndex);});
   connect(this, &QInstaller::pipInstallFinished, this, &QInstaller::handlePipInstallFinished);
   connect(this, &QInstaller::installationSuccess,this, &QInstaller::handleInstallationSuccess);
   connect(this, &QInstaller::installationFailed, this, &QInstaller::handleInstallationFailed);

}
void QInstaller::handlePageChanged()
{
    if(_topWidget->currentPage() == PageController::Page::INSTALLATION_PAGE)
    {
        _bottomWidget->setRightButtonLabel("Install");
        _bottomWidget->setRightButtonEnabled(false);
        _bottomWidget->setLeftButtonVisible(true);
    }

    if(_topWidget->currentPage() == PageController::Page::WELCOME_PAGE)
    {
        _bottomWidget->setRightButtonLabel("Next");
        _bottomWidget->setRightButtonEnabled(true);
        _bottomWidget->setLeftButtonVisible(false);
    }
}

void QInstaller::handlePipInstallFinished(const int &code)
{
    if(code == static_cast<int>(EXIT_CODE::OK))
    {
       emit installationSuccess();
    } else
    {
        emit installationFailed();
    }
}

void QInstaller::handlePackageSelected(const int& selectedIndex)
{
    if(!selectedIndex)
    {
        if(_bottomWidget->isRightButtonEnabled())
            _bottomWidget->setRightButtonEnabled(false);
    }

    if(selectedIndex)
        _bottomWidget->setRightButtonEnabled(true);

    _selectedIndex = selectedIndex;


}

void QInstaller::handleInstallButtonPressed()
{
    if(_selectedIndex)
    {
        _bottomWidget->setRightButtonEnabled(false);



        switch(static_cast<Packages>(_selectedIndex))
        {
            case Packages::PIP : handlePipInstallation(); handleBusyScreen(); break;
        }
    }
}

void QInstaller::handlePipInstallation()
{
    QProcess *proc = new QProcess(this);

    QString cmd = "python3 -m pip install --user requests";

    proc->setProgram("bash");
    proc->setArguments({"-lc", cmd});
    proc->setProcessChannelMode(QProcess::MergedChannels);

    connect(proc, &QProcess::readyRead, this, [proc]() {
        qDebug() << proc->readAll();
    });

    connect(proc,
            QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this,
            [this,proc](int code, QProcess::ExitStatus status) {
                qDebug() << "Finished with code:" << code
                         << "status:" << status;
                emit pipInstallFinished(code);
                proc->deleteLater();
            });

    proc->start();
}

void QInstaller::handleBusyScreen()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Message");
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    QLabel *label = new QLabel("Installation in process...", dialog);
    label->setWordWrap(true);
    layout->addWidget(label);

    connect(this, &QInstaller::pipInstallFinished,dialog, &QDialog::accept );

    dialog->exec();
}

void QInstaller::handleInstallationSuccess()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Message");
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    QLabel *label = new QLabel("Installation completed successfully!", dialog);
    label->setWordWrap(true);
    layout->addWidget(label);

    QPushButton *okButton = new QPushButton("OK", dialog);
    layout->addWidget(okButton);
    okButton->setDefault(true);

    connect(okButton, &QPushButton::clicked, dialog, &QDialog::accept);

    connect(dialog, &QDialog::accepted, this, [this, dialog]() {

       _topWidget->setCurrentPackageIndex(static_cast<int>(Packages::NOT_SELECTED));
        dialog->deleteLater();
    });

    dialog->exec();
}

void QInstaller::handleInstallationFailed()
{

}

