#include "macroiconsgraphicsview.h"
#include <QDebug>
#include <QMouseEvent>
#include <QXmlStreamReader>
#include <QGraphicsRectItem>
#include "macroitemrect.h"

MacroIconsGraphicsView::MacroIconsGraphicsView(QWidget *parent) :
    QGraphicsView(parent), currentSelectedIcon(NULL) {
}

void MacroIconsGraphicsView::mousePressEvent(QMouseEvent *event) {
    QGraphicsView::mousePressEvent(event);
}

void MacroIconsGraphicsView::iconClicked(MacroItemRect* name) {
    if (currentSelectedIcon == name)
        return;

    if (currentSelectedIcon != NULL) {
        currentSelectedIcon->unselectIcon();
        currentSelectedIcon = NULL;
    }

    currentSelectedIcon = name;
}

void MacroIconsGraphicsView::setCurrentIcon(const QString& name) {
    MacroItemRect* icon = icons.value(name, NULL);

    if (icon != NULL)  {
        icon->selectIcon();
    }
}

void MacroIconsGraphicsView::parseIconMetadata() {
    QFile file(":/files/icons.xml");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        /*QMessageBox::critical(this,
                              "QXSRExample::parseXML",
                              "Couldn't open example.xml",
                              QMessageBox::Ok);*/

        qDebug() << "could not open icons.xml";
        return;
    }

    QXmlStreamReader xml(&file);

    while(!xml.atEnd() &&
          !xml.hasError()) {

        QXmlStreamReader::TokenType token = xml.readNext();
        if(token == QXmlStreamReader::StartDocument) {
            continue;
        }

        if(token == QXmlStreamReader::StartElement) {
            //qDebug() << xml.name() << " data " <<  xml.attributes().size();

            if (xml.name() != "ImageStyle") {
                continue;
            }

            QXmlStreamAttributes attributes = xml.attributes();
            QString source = attributes.value("Source").toString();

            if (source != "ui_rebel_icons")
                continue;

            QString sourceRect = attributes.value("SourceRect").toString();
            QString name = attributes.value("Name").toString();

            if (sourceRect.isEmpty())
                continue;

            QStringList rectValues = sourceRect.split(",");
            if (rectValues.size() < 4)
                continue;

            //            int x = rectValues.at(0).toInt();
            //            int y = rectValues.at(1).toInt();

            //   int hash = ((x << 16) & 0xFFFF0000) | y;

            MacroItemRect* rectangle = new MacroItemRect(name, rectValues.at(0).toInt(),
                                                         rectValues.at(1).toInt(),
                                                         rectValues.at(2).toInt() - rectValues.at(0).toInt(),
                                                         rectValues.at(3).toInt() - rectValues.at(1).toInt());

            connect(rectangle, SIGNAL(iconClicked(MacroItemRect*)), this, SLOT(iconClicked(MacroItemRect*)));

            icons.insert(name, rectangle);
            //rectangle->set
            QPen pen = rectangle->pen();
            pen.setWidth(2);
            pen.setStyle(Qt::NoPen);
            pen.setColor(Qt::white);
            rectangle->setPen(pen);

            this->scene()->addItem(rectangle);
            //          }
        }
    }

    if (xml.hasError()) {
        qDebug() << "xml error:" << xml.errorString();
    }

}
