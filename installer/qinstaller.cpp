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
   connect(_topWidget, &PageController::packageSelected,this, &QInstaller::handlePackageSelected);
   // connect(this, &QInstaller::installationFinished, this, &QInstaller::handleInstallationFinished);
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

// void QInstaller::handleInstallationFinished(const int& code)
// {
//     const bool success =
//             (static_cast<EXIT_CODE>(code) == EXIT_CODE::OK);

//     QString message = success ? QString("%1 installation completed successfully!")
//                                 .arg(packageToString(static_cast<Packages>(_selectedIndex)))
//                           : QString("%1 installation failed.")
//                                 .arg(packageToString(static_cast<Packages>(_selectedIndex)));

//     QDialog *dialog = new QDialog(this);
//     dialog->setWindowTitle("Message");
//     QVBoxLayout *layout = new QVBoxLayout(dialog);
//     QLabel *label = new QLabel(message, dialog);
//     label->setWordWrap(true);
//     layout->addWidget(label);

//     QPushButton *okButton = new QPushButton("OK", dialog);
//     layout->addWidget(okButton);
//     okButton->setDefault(true);

//     connect(okButton, &QPushButton::clicked, dialog, &QDialog::accept);

//     connect(dialog, &QDialog::accepted, this, [this, dialog]() {

//        _topWidget->setCurrentPackageIndex(static_cast<int>(Packages::NOT_SELECTED));
//         dialog->deleteLater();
//     });

//     dialog->exec();
// }

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
        handleBusyScreen();
        handleInstallation(static_cast<Packages>(_selectedIndex));
    }
}

void QInstaller::handleInstallation(const Packages& selectedPackage)
{
    QString cmd;

    switch(selectedPackage)
    {
        case Packages::PIP:
            cmd = "python3 -m pip install --user requests";
            break;
        case Packages::NPM:
            cmd ="npm install --prefix ~/.local lodash" ;
            break;
        case Packages::NOT_SELECTED:
        default:
            return;
    }

    QProcess *proc = new QProcess(this);

    proc->setProgram("bash");
    proc->setArguments({"-lc", cmd});
    proc->setProcessChannelMode(QProcess::MergedChannels);

    connect(proc, &QProcess::readyRead, this, [proc]() {
        qDebug() << proc->readAll();
    });

    connect(proc,
            QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this,
            [this,proc,selectedPackage](int code, QProcess::ExitStatus status) {
                qDebug() << "Finished with code:" << code
                         << "_status:" << status;
                emit installationFinished(code);
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

    QPushButton *okButton = new QPushButton("OK", dialog);
    layout->addWidget(okButton);
    okButton->setDefault(true);
    okButton->hide();

    connect(okButton, &QPushButton::clicked, dialog, &QDialog::accept);

    connect(dialog, &QDialog::accepted, this, [this, dialog]() {

       _topWidget->setCurrentPackageIndex(static_cast<int>(Packages::NOT_SELECTED));
        dialog->deleteLater();
    });

    connect(this, &QInstaller::installationFinished,dialog, [this, dialog, label, okButton](const int& code) {
        const bool success =
                (static_cast<EXIT_CODE>(code) == EXIT_CODE::OK);

        QString message = success ? QString("%1 installation completed successfully!")
                                    .arg(packageToString(static_cast<Packages>(_selectedIndex)))
                              : QString("%1 installation failed.")
                                    .arg(packageToString(static_cast<Packages>(_selectedIndex)));
        label->setText(message);

        okButton->show();
        dialog->adjustSize();
    });

    dialog->show();
}

QString QInstaller::packageToString(const Packages& pkg)
{
    switch (pkg) {
     case Packages::PIP:  return "PIP package";
     case Packages::NPM:  return "NPM package";
     case Packages::NOT_SELECTED:
     default:
         return "Unknown package";
     }
}




