#include "macroitemrect.h"
#include <QDebug>
#include <QPen>

MacroItemRect::MacroItemRect(const QString& iconName, qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent) :
    QGraphicsRectItem(x, y, width, height, parent), iconName(iconName) {
}

void MacroItemRect::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    selectIcon();

    QGraphicsRectItem::mousePressEvent(event);
}

void MacroItemRect::selectIcon() {
    qDebug() << iconName << " selected";

    QPen pen = this->pen();
    pen.setStyle(Qt::SolidLine);
    this->setPen(pen);

    emit iconClicked(this);
}

void MacroItemRect::unselectIcon() {
    QPen pen = this->pen();
    pen.setStyle(Qt::NoPen);
    this->setPen(pen);
}
