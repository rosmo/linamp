#include "queuecontrolbuttonswidget.h"
#include "ui_queuecontrolbuttonswidget.h"
#include "scale.h"

#include <QFile>

QueueControlButtonsWidget::QueueControlButtonsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QueueControlButtonsWidget)

{
    ui->setupUi(this);

    // Add menu
    addMenu = new QueueControlMenu(this);
    addMenu->setProperty("class", "add");
    removeMenu = new QueueControlMenu(this);
    removeMenu->setProperty("class", "remove");
    selectMenu = new QueueControlMenu(this);
    selectMenu->setProperty("class", "select");
    miscMenu = new QueueControlMenu(this);
    miscMenu->setProperty("class", "misc");
    optionsMenu = new QueueControlMenu(this);
    optionsMenu->setProperty("class", "options");

    QMapIterator<QString, QueueControlIcon> iter(iconMap);
    while (iter.hasNext()) {
        iter.next();
        QueueControlIcon menuAndAction = iter.value();

        QIcon icon(":/assets/" + menuAndAction.icon + ".png");
        icon.addFile(":/assets/" + menuAndAction.icon + "_p.png", QSize(), QIcon::Active);
        QAction **action = menuAndAction.action;
        QueueControlMenu **menu = menuAndAction.menu;
        *action = new QAction(icon, QString(""), this);
        (*menu)->addAction(*action);
    }

    addMenu->setStyleSheet(getStylesheet("queuecontrolbuttons.menu"));
    removeMenu->setStyleSheet(getStylesheet("queuecontrolbuttons.menu"));
    selectMenu->setStyleSheet(getStylesheet("queuecontrolbuttons.menu"));
    miscMenu->setStyleSheet(getStylesheet("queuecontrolbuttons.menu"));
    optionsMenu->setStyleSheet(getStylesheet("queuecontrolbuttons.menu"));

    scale();

    ui->addButton->setMenu(addMenu);
    ui->addButton->setPopupMode(QToolButton::InstantPopup);
    ui->removeButton->setMenu(removeMenu);
    ui->removeButton->setPopupMode(QToolButton::InstantPopup);
    ui->selectButton->setMenu(selectMenu);
    ui->selectButton->setPopupMode(QToolButton::InstantPopup);
    ui->miscButton->setMenu(miscMenu);
    ui->miscButton->setPopupMode(QToolButton::InstantPopup);
    ui->optionsButton->setMenu(optionsMenu);
    ui->optionsButton->setPopupMode(QToolButton::InstantPopup);

    connect(ui->addButton, &QToolButton::clicked, this, &QueueControlButtonsWidget::addClicked);
    connect(ui->removeButton, &QToolButton::clicked, this, &QueueControlButtonsWidget::removeClicked);
    connect(ui->selectButton, &QToolButton::clicked, this, &QueueControlButtonsWidget::selectClicked);
    connect(ui->miscButton, &QToolButton::clicked, this, &QueueControlButtonsWidget::miscClicked);
    connect(ui->optionsButton, &QToolButton::clicked, this, &QueueControlButtonsWidget::optionsClicked);
}

QueueControlButtonsWidget::~QueueControlButtonsWidget()
{
    delete ui;
}

void QueueControlButtonsWidget::scale()
{
    printf("SCALING BUTTONS!\n");

    //QSize iconSize = addMenu->actions().first()->icon().actualSize(QSize(10000, 10000));
    //printf("Resizing menu to: %d %d", iconSize.width(), iconSize.height() * addMenu->actions().count());
    //addMenu->resize(iconSize.width(), iconSize.height() * addMenu->actions().count());

    ui->addButton->setMaximumWidth(ui->addButton->maximumWidth() * UI_SCALE);
    ui->addButton->setMinimumWidth(ui->addButton->minimumWidth() * UI_SCALE);
    ui->addButton->setMaximumHeight(ui->addButton->maximumHeight() * UI_SCALE);
    ui->addButton->setMinimumHeight(ui->addButton->minimumHeight() * UI_SCALE);

    ui->removeButton->setMaximumWidth(ui->removeButton->maximumWidth() * UI_SCALE);
    ui->removeButton->setMinimumWidth(ui->removeButton->minimumWidth() * UI_SCALE);
    ui->removeButton->setMaximumHeight(ui->removeButton->maximumHeight() * UI_SCALE);
    ui->removeButton->setMinimumHeight(ui->removeButton->minimumHeight() * UI_SCALE);

    ui->selectButton->setMaximumWidth(ui->selectButton->maximumWidth() * UI_SCALE);
    ui->selectButton->setMinimumWidth(ui->selectButton->minimumWidth() * UI_SCALE);
    ui->selectButton->setMaximumHeight(ui->selectButton->maximumHeight() * UI_SCALE);
    ui->selectButton->setMinimumHeight(ui->selectButton->minimumHeight() * UI_SCALE);

    ui->miscButton->setMaximumWidth(ui->miscButton->maximumWidth() * UI_SCALE);
    ui->miscButton->setMinimumWidth(ui->miscButton->minimumWidth() * UI_SCALE);
    ui->miscButton->setMaximumHeight(ui->miscButton->maximumHeight() * UI_SCALE);
    ui->miscButton->setMinimumHeight(ui->miscButton->minimumHeight() * UI_SCALE);

    ui->optionsButton->setMaximumWidth(ui->optionsButton->maximumWidth() * UI_SCALE);
    ui->optionsButton->setMinimumWidth(ui->optionsButton->minimumWidth() * UI_SCALE);
    ui->optionsButton->setMaximumHeight(ui->optionsButton->maximumHeight() * UI_SCALE);
    ui->optionsButton->setMinimumHeight(ui->optionsButton->minimumHeight() * UI_SCALE);

    ui->queueControlsContainer->setMaximumWidth(ui->queueControlsContainer->maximumWidth() * UI_SCALE);
    ui->queueControlsContainer->setMinimumWidth(ui->queueControlsContainer->minimumWidth() * UI_SCALE);
    ui->queueControlsContainer->setMaximumHeight(ui->queueControlsContainer->maximumHeight() * UI_SCALE);
    ui->queueControlsContainer->setMinimumHeight(ui->queueControlsContainer->minimumHeight() * UI_SCALE);
    
    this->setMaximumWidth(this->maximumWidth() * UI_SCALE);
    this->setMinimumWidth(this->minimumWidth() * UI_SCALE);
    this->setMaximumHeight(this->maximumHeight() * UI_SCALE);
    this->setMinimumHeight(this->minimumHeight() * UI_SCALE);
}
