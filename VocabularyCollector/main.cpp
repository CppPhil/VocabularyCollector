#include "stdafx.h"
#include "vocabularycollector.h"
#include <QtWidgets/QApplication>

/*
    TODO:
         remove items from list
         make exporting to file prettier
         make printing prettier
         import from file
         require http lookup to be enabled specifically
*/

int main(int argc, char *argv[]) {
    QApplication application{ argc, argv };
    VocabularyCollector window{ };
    window.show();
    return application.exec();
}
