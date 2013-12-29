#include "gamemacro.h"
#include <QDebug>

GameMacro::GameMacro() {
}

bool GameMacro::parseMacro(const QString& macroString) {
    int idIndex = macroString.indexOf(" ");

    if (idIndex <= 0)
        return false;

    QString id = macroString.mid(0, idIndex);

    this->id = id.toInt();

    int nameIndex = macroString.indexOf(" ", idIndex + 1);

    if (nameIndex <= 0)
        return false;

    name = macroString.mid(idIndex + 1, nameIndex - (idIndex + 1));

    this->setText(name);

    qDebug() << "detected macro: " << name;

    int iconIndex = macroString.indexOf(" ", nameIndex + 1);

    if (iconIndex <= 0)
        return false;

    icon = macroString.mid(nameIndex + 1, iconIndex - (nameIndex + 1));

    qDebug() << "icon: " << icon;

    int colorIndex = macroString.indexOf(" ", iconIndex + 1);

    if (colorIndex <= 0)
        return false;

    color = macroString.mid(iconIndex + 1, colorIndex - (iconIndex + 1));

    qDebug() << "color: " << color;

   /* int contentsIndex = macroString.indexOf(" ", colorIndex + 1);

    if (contentsIndex <= 0)
        return false;*/

    contents = macroString.mid(colorIndex + 1);

    qDebug() << "contents: " << contents;

    qDebug() << "serialized: " << toString();

    return true;
    //names.append(name);
}

QString GameMacro::toString() {
    QString str;
    QTextStream stream(&str);

    stream << id << " " << name << " " << icon << " " << color << " " << contents;

    return str;
}
