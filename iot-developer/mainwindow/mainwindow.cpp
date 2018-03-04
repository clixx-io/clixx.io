/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QAction>
#include <QLayout>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QTextEdit>
#include <QFile>
#include <QDataStream>
#include <QFileDialog>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QSignalMapper>
#include <QApplication>
#include <QPainter>
#include <QMouseEvent>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QDebug>
#include <QDockWidget>
#include <QListWidget>
#include <QTreeWidget>
#include <QInputDialog>

#include "codeeditor.h"
#include "communicatorserialwidget.h"
#include "projectwidget.h"
#include "clixxiotprojects.h"
#include "mainwindow.h"
#include "colorswatch.h"
#include "toolbar.h"

static const char message[] =
    "<p><b>Clixx.io Development IDE</b></p>"

    "<p>This is the Development IDE for Clixx.io Software.</p>"

    "<p>The tool bar and dock widgets can be dragged around and rearranged "
    "using the mouse or via the menu.</p>"

    "<p>Each dock widget contains a colored frame and a context "
    "(right-click) menu.</p>"

#ifdef Q_OS_MAC
    "<p>On OS X, the \"Black\" dock widget has been created as a "
    "<em>Drawer</em>, which is a special kind of QDockWidget.</p>"
#endif
    ;

Q_DECLARE_METATYPE(QDockWidget::DockWidgetFeatures)

MainWindow::MainWindow(const CustomSizeHintMap &customSizeHints,
                       QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags)
{
    setObjectName("MainWindow");

    setWindowTitle("Clixx.IO IoT Developer");
    Projects = new ClixxIoTProjects();
    currentProject = new ClixxIoTProject();

    setupToolBar();
    setupMenuBar();
    setupDockWidgets(customSizeHints);

    statusBar()->showMessage(tr("Status Bar"));
}

void MainWindow::actionTriggered(QAction *action)
{
    qDebug("action '%s' triggered", action->text().toLocal8Bit().data());
}

void MainWindow::setupToolBar()
{
#ifdef Q_OS_OSX
    setUnifiedTitleAndToolBarOnMac(true);
#endif

    ToolBar *tb = new ToolBar(QString::fromLatin1("Tool Bar 1"), this);
    toolBars.append(tb);
    addToolBar(tb);

}

void MainWindow::setupMenuBar()
{
    QMenu *menu = menuBar()->addMenu(tr("&File"));

    menu->addAction(tr("New Project.."),this, &MainWindow::switchLayoutDirection);
    menu->addAction(tr("Load Project.."), this, &MainWindow::loadProject);
    menu->addAction(tr("Save layout.."), this, &MainWindow::saveLayout);

    menu->addSeparator();
    menu->addAction(tr("&Quit"), this, &QWidget::close);

    // FileMenu = menuBar()->addMenu(tr("&File"));
    EditMenu = menuBar()->addMenu(tr("&Edit"));
    // HelpMenu = menuBar()->addMenu(tr("&Help"));

    mainWindowMenu = menuBar()->addMenu(tr("&Hardware"));

    QAction *action = mainWindowMenu->addAction(tr("Animated docks"));
    action->setCheckable(true);
    action->setChecked(dockOptions() & AnimatedDocks);
    connect(action, &QAction::toggled, this, &MainWindow::setDockOptions);

    action = mainWindowMenu->addAction(tr("Allow nested docks"));
    action->setCheckable(true);
    action->setChecked(dockOptions() & AllowNestedDocks);
    connect(action, &QAction::toggled, this, &MainWindow::setDockOptions);

    action = mainWindowMenu->addAction(tr("Allow tabbed docks"));
    action->setCheckable(true);
    action->setChecked(dockOptions() & AllowTabbedDocks);
    connect(action, &QAction::toggled, this, &MainWindow::setDockOptions);

    action = mainWindowMenu->addAction(tr("Force tabbed docks"));
    action->setCheckable(true);
    action->setChecked(dockOptions() & ForceTabbedDocks);
    connect(action, &QAction::toggled, this, &MainWindow::setDockOptions);

    action = mainWindowMenu->addAction(tr("Vertical tabs"));
    action->setCheckable(true);
    action->setChecked(dockOptions() & VerticalTabs);
    connect(action, &QAction::toggled, this, &MainWindow::setDockOptions);

    action = mainWindowMenu->addAction(tr("Grouped dragging"));
    action->setCheckable(true);
    action->setChecked(dockOptions() & GroupedDragging);
    connect(action, &QAction::toggled, this, &MainWindow::setDockOptions);

    QMenu *toolBarMenu = menuBar()->addMenu(tr("&Software"));
    for (int i = 0; i < toolBars.count(); ++i)
        toolBarMenu->addMenu(toolBars.at(i)->toolbarMenu());

#ifdef Q_OS_OSX
    toolBarMenu->addSeparator();

    action = toolBarMenu->addAction(tr("Unified"));
    action->setCheckable(true);
    action->setChecked(unifiedTitleAndToolBarOnMac());
    connect(action, &QAction::toggled, this, &QMainWindow::setUnifiedTitleAndToolBarOnMac);
#endif

    NetworkMenu = menuBar()->addMenu(tr("&Network"));
    dockWidgetMenu = menuBar()->addMenu(tr("&Help"));

}

void MainWindow::setDockOptions()
{
    DockOptions opts;
    QList<QAction*> actions = mainWindowMenu->actions();

    if (actions.at(0)->isChecked())
        opts |= AnimatedDocks;
    if (actions.at(1)->isChecked())
        opts |= AllowNestedDocks;
    if (actions.at(2)->isChecked())
        opts |= AllowTabbedDocks;
    if (actions.at(3)->isChecked())
        opts |= ForceTabbedDocks;
    if (actions.at(4)->isChecked())
        opts |= VerticalTabs;
    if (actions.at(5)->isChecked())
        opts |= GroupedDragging;

    QMainWindow::setDockOptions(opts);
}

void MainWindow::saveLayout()
{
    QString fileName
        = QFileDialog::getSaveFileName(this, tr("Save layout"));
    if (fileName.isEmpty())
        return;
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly)) {
        QString msg = tr("Failed to open %1\n%2")
                        .arg(QDir::toNativeSeparators(fileName), file.errorString());
        QMessageBox::warning(this, tr("Error"), msg);
        return;
    }

    QByteArray geo_data = saveGeometry();
    QByteArray layout_data = saveState();

    bool ok = file.putChar((uchar)geo_data.size());
    if (ok)
        ok = file.write(geo_data) == geo_data.size();
    if (ok)
        ok = file.write(layout_data) == layout_data.size();

    if (!ok) {
        QString msg = tr("Error writing to %1\n%2")
                        .arg(QDir::toNativeSeparators(fileName), file.errorString());
        QMessageBox::warning(this, tr("Error"), msg);
        return;
    }
}

void MainWindow::loadProject()
{

    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 Projects->getProjectsDir(),
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);

    if (dir.length())
        projectWindow->LoadProject(dir);

    return;

}

static QAction *addCornerAction(const QString &text, QMainWindow *mw, QMenu *menu, QActionGroup *group,
                                Qt::Corner c, Qt::DockWidgetArea a)
{
    QAction *result = menu->addAction(text, mw, [=]() { mw->setCorner(c, a); });
    result->setCheckable(true);
    group->addAction(result);
    return result;
}

void MainWindow::setupDockWidgets(const CustomSizeHintMap &customSizeHints)
{

    // Compiler Output Area
    QDockWidget *dock = new QDockWidget(tr("Output"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    QListWidget *outputList = new QListWidget(dock);
    outputList->addItems(QStringList()
            << "Ready."
//          << "Jane Doe, Memorabilia, 23 Watersedge, Beaton"
            );
    dock->setWidget(outputList);
    addDockWidget(Qt::BottomDockWidgetArea, dock);
    //dock->setMinimumHeight(300);
    dock->setMinimumWidth(400);
    dock->show();

    // Project Window
    QDockWidget *myproject = new QDockWidget(tr("Project"),this);
    projectWindow = new ProjectWidget(myproject);
    myproject->setMinimumWidth(200);
    myproject->setMinimumHeight(100);
    addDockWidget(Qt::LeftDockWidgetArea, myproject);
    projectWindow->setMainWindow(this);
    myproject->show();

    // A Multipurpose communicator
    QDockWidget *myframe = new QDockWidget(tr("Communicator"),this);
    commWindow = new CommunicatorSerialWidget(myframe);
    addDockWidget(Qt::RightDockWidgetArea, myframe);
    myframe->setMinimumWidth(400);
    myframe->setMinimumHeight(300);
    myframe->show();

    // Central Code Editor area
    center = new CodeEditor(this);
    center->setMinimumSize(400, 205);
    setCentralWidget(center);

    return;

    qRegisterMetaType<QDockWidget::DockWidgetFeatures>();

    QMenu *cornerMenu = dockWidgetMenu->addMenu(tr("Top left corner"));
    QActionGroup *group = new QActionGroup(this);
    group->setExclusive(true);
    QAction *cornerAction = addCornerAction(tr("Top dock area"), this, cornerMenu, group, Qt::TopLeftCorner, Qt::TopDockWidgetArea);
    cornerAction->setChecked(true);
    addCornerAction(tr("Left dock area"), this, cornerMenu, group, Qt::TopLeftCorner, Qt::LeftDockWidgetArea);

    cornerMenu = dockWidgetMenu->addMenu(tr("Top right corner"));
    group = new QActionGroup(this);
    group->setExclusive(true);
    cornerAction = addCornerAction(tr("Top dock area"), this, cornerMenu, group, Qt::TopRightCorner, Qt::TopDockWidgetArea);
    cornerAction->setChecked(true);
    addCornerAction(tr("Right dock area"), this, cornerMenu, group, Qt::TopRightCorner, Qt::RightDockWidgetArea);

    cornerMenu = dockWidgetMenu->addMenu(tr("Bottom left corner"));
    group = new QActionGroup(this);
    group->setExclusive(true);
    cornerAction = addCornerAction(tr("Bottom dock area"), this, cornerMenu, group, Qt::BottomLeftCorner, Qt::BottomDockWidgetArea);
    cornerAction->setChecked(true);
    addCornerAction(tr("Left dock area"), this, cornerMenu, group, Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);

    cornerMenu = dockWidgetMenu->addMenu(tr("Bottom right corner"));
    group = new QActionGroup(this);
    group->setExclusive(true);
    cornerAction = addCornerAction(tr("Bottom dock area"), this, cornerMenu, group, Qt::BottomRightCorner, Qt::BottomDockWidgetArea);
    cornerAction->setChecked(true);
    addCornerAction(tr("Right dock area"), this, cornerMenu, group, Qt::BottomRightCorner, Qt::RightDockWidgetArea);

    dockWidgetMenu->addSeparator();

    static const struct Set {
        const char * name;
        uint flags;
        Qt::DockWidgetArea area;
    } sets [] = {
#ifndef Q_OS_MAC
        { "Black", 0, Qt::LeftDockWidgetArea },
#else
        { "Black", Qt::Drawer, Qt::LeftDockWidgetArea },
#endif
        { "White", 0, Qt::RightDockWidgetArea },
        { "Red", 0, Qt::TopDockWidgetArea },
        { "Green", 0, Qt::TopDockWidgetArea },
        { "Blue", 0, Qt::BottomDockWidgetArea },
        { "Yellow", 0, Qt::BottomDockWidgetArea }
    };
    const int setCount = sizeof(sets) / sizeof(Set);

    const QIcon qtIcon(QPixmap(":/res/qt.png"));
    for (int i = 0; i < setCount; ++i) {
        ColorSwatch *swatch = new ColorSwatch(tr(sets[i].name), this, Qt::WindowFlags(sets[i].flags));
        if (i % 2)
            swatch->setWindowIcon(qtIcon);
        if (qstrcmp(sets[i].name, "Blue") == 0) {
            BlueTitleBar *titlebar = new BlueTitleBar(swatch);
            swatch->setTitleBarWidget(titlebar);
            connect(swatch, &QDockWidget::topLevelChanged, titlebar, &BlueTitleBar::updateMask);
            connect(swatch, &QDockWidget::featuresChanged, titlebar, &BlueTitleBar::updateMask, Qt::QueuedConnection);
        }

        QString name = QString::fromLatin1(sets[i].name);
        if (customSizeHints.contains(name))
            swatch->setCustomSizeHint(customSizeHints.value(name));

        addDockWidget(sets[i].area, swatch);
        dockWidgetMenu->addMenu(swatch->colorSwatchMenu());
    }

    destroyDockWidgetMenu = new QMenu(tr("Destroy dock widget"), this);
    destroyDockWidgetMenu->setEnabled(false);
    connect(destroyDockWidgetMenu, &QMenu::triggered, this, &MainWindow::destroyDockWidget);

    dockWidgetMenu->addSeparator();
    dockWidgetMenu->addAction(tr("Add dock widget..."), this, &MainWindow::createDockWidget);
    dockWidgetMenu->addMenu(destroyDockWidgetMenu);
}

void MainWindow::switchLayoutDirection()
{
    if (layoutDirection() == Qt::LeftToRight)
        QApplication::setLayoutDirection(Qt::RightToLeft);
    else
        QApplication::setLayoutDirection(Qt::LeftToRight);
}

class CreateDockWidgetDialog : public QDialog
{
public:
    explicit CreateDockWidgetDialog(QWidget *parent = Q_NULLPTR);

    QString enteredObjectName() const { return m_objectName->text(); }
    Qt::DockWidgetArea location() const;

private:
    QLineEdit *m_objectName;
    QComboBox *m_location;
};

CreateDockWidgetDialog::CreateDockWidgetDialog(QWidget *parent)
    : QDialog(parent)
    , m_objectName(new QLineEdit(this))
    , m_location(new QComboBox(this))
{
    setWindowTitle(tr("Add Dock Widget"));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    QGridLayout *layout = new QGridLayout(this);

    layout->addWidget(new QLabel(tr("Object name:")), 0, 0);
    layout->addWidget(m_objectName, 0, 1);

    layout->addWidget(new QLabel(tr("Location:")), 1, 0);
    m_location->setEditable(false);
    m_location->addItem(tr("Top"));
    m_location->addItem(tr("Left"));
    m_location->addItem(tr("Right"));
    m_location->addItem(tr("Bottom"));
    m_location->addItem(tr("Restore"));
    layout->addWidget(m_location, 1, 1);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    layout->addWidget(buttonBox, 2, 0, 1, 2);
}

Qt::DockWidgetArea CreateDockWidgetDialog::location() const
{
    switch (m_location->currentIndex()) {
        case 0: return Qt::TopDockWidgetArea;
        case 1: return Qt::LeftDockWidgetArea;
        case 2: return Qt::RightDockWidgetArea;
        case 3: return Qt::BottomDockWidgetArea;
        default:
            break;
    }
    return Qt::NoDockWidgetArea;
}

void MainWindow::createDockWidget()
{
    CreateDockWidgetDialog dialog(this);
    if (dialog.exec() == QDialog::Rejected)
        return;

    QDockWidget *dw = new QDockWidget;
    const QString name = dialog.enteredObjectName();
    dw->setObjectName(name);
    dw->setWindowTitle(name);
    dw->setWidget(new QTextEdit);

    Qt::DockWidgetArea area = dialog.location();
    switch (area) {
        case Qt::LeftDockWidgetArea:
        case Qt::RightDockWidgetArea:
        case Qt::TopDockWidgetArea:
        case Qt::BottomDockWidgetArea:
            addDockWidget(area, dw);
            break;
        default:
            if (!restoreDockWidget(dw)) {
                QMessageBox::warning(this, QString(), tr("Failed to restore dock widget"));
                delete dw;
                return;
            }
            break;
    }

    extraDockWidgets.append(dw);
    destroyDockWidgetMenu->setEnabled(true);
    destroyDockWidgetMenu->addAction(new QAction(name, this));
}

void MainWindow::destroyDockWidget(QAction *action)
{
    int index = destroyDockWidgetMenu->actions().indexOf(action);
    delete extraDockWidgets.takeAt(index);
    destroyDockWidgetMenu->removeAction(action);
    action->deleteLater();

    if (destroyDockWidgetMenu->isEmpty())
        destroyDockWidgetMenu->setEnabled(false);
}

void MainWindow::LoadCodeSource(const QString filename)
{
    QFile file(filename);

    file.open(QFile::ReadOnly | QFile::Text);
    center->setPlainText(file.readAll());

}
