#include "editmacro.h"
#include "ui_editmacro.h"

#include <QGraphicsPixmapItem>

EditMacro::EditMacro(GameMacro *macro, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditMacro), macro(macro) {
    ui->setupUi(this);

    scene = new QGraphicsScene();
    scene->addItem(new QGraphicsPixmapItem(QPixmap::fromImage(getIconsImage())));
    ui->graphicsView->setScene(scene);

    ui->graphicsView->setBackgroundBrush(QBrush(QColor(0, 174, 255)));

    ui->graphicsView->parseIconMetadata();
    ui->graphicsView->setCurrentIcon(macro->getIcon());
    ui->lineEdit_name->setText(macro->getName());
    ui->plainTextEdit_contents->appendPlainText(macro->getContents());
}

EditMacro::~EditMacro() {
    delete ui;
}

QImage EditMacro::getIconsImage() {
    return QImage(":/img/ui_rebel_icons.png");
}
