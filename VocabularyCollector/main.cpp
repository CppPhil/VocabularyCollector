#include "stdafx.h"
#include "vocabularycollector.h"
#include <QtWidgets/QApplication>

/*
    TODO:
         make exporting to file prettier
         make printing prettier
*/

int main(int argc, char *argv[]) {
    QApplication application{ argc, argv };
    VocabularyCollector window{ };
    window.show();
    return application.exec();
}
