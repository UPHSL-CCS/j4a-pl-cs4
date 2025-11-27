#include <QApplication>
#include "modules/pokeui.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    PokedexUI window;
    window.show();
    return app.exec();
}

