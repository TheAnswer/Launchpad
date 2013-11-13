#include "statusxmlcontenthandler.h"
#include <iostream>
#include "mainwindow.h"

StatusXmlContentHandler::StatusXmlContentHandler(MainWindow* mainWindow) : mainWindow(mainWindow) {
}

StatusXmlContentHandler::~StatusXmlContentHandler() {
  //qDebug() << "deleting handler";
}

bool StatusXmlContentHandler::startElement(const QString & , const QString & ,
                                        const QString & qName, const QXmlAttributes &  ) {

  //std::cout << qName.toStdString() << ": ";

  currentElement = qName;
  currentValue = "";

  return true;
}

bool StatusXmlContentHandler::endElement ( const QString & , const QString & , const QString &  ) {

  if (currentElement.isEmpty())
    return true;

  values.insert(currentElement, currentValue);

  currentElement = "";
  currentValue = "";
  return true;
}

bool StatusXmlContentHandler::characters ( const QString & ch ) {
  currentValue = ch;

  return true;
}
