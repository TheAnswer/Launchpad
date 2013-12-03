#include "utils.h"
#include <QDebug>

Utils::Utils() {
}

QStringList Utils::getArgumentList(const QString& args) {
    bool str = false;
    QStringList argumentList;
    QString currentArg;

    for (int i = 0; i < args.length(); ++i) {
        currentArg.append(args.at(i));

        if (args.at(i) == '"' || args.at(i) == '\'') {
            if (str) {
                argumentList.append(currentArg);
                currentArg = "";
            }

            str = !str;
        } else if (args.at(i) == ' ') {
            if (str)
                continue;
            else {
                currentArg.remove(currentArg.length() - 1, 1);

                if (!currentArg.isEmpty())
                    argumentList.append(currentArg);

                currentArg = "";
            }
        }
    }

    if (!currentArg.isEmpty())
        argumentList.append(currentArg);

    return argumentList;
}
