#ifndef pokeui_h
#define pokeui_h

#include "pokedata.h"  // Your Pokemon/Region structs
#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QListWidget>
#include <QSplitter>
#include <QFuture>
#include <QPixmap>

class PokedexUI : public QMainWindow {
    Q_OBJECT

public:
    PokedexUI(QWidget *parent = nullptr);
    ~PokedexUI();

private slots:
    void onSearchClicked();
    void onListClicked();
    void onRegionTabChanged(int index);
    void onPokemonSelected(QListWidgetItem *item);
    void displayPokemon(const Pokemon& p);
    void displayError(const QString& msg);
    void loadRegions();

private:
    void setupUI();
    void fetchAndDisplay(const QString& query);

    QLineEdit *searchInput;
    QPushButton *searchButton;
    QPushButton *listButton;
    QTabWidget *regionTabs;
    QListWidget *pokemonList;
    QLabel *nameLabel;
    QLabel *idLabel;
    QLabel *typesLabel;
    QLabel *heightLabel;
    QLabel *weightLabel;
    QLabel *descriptionLabel;
    QLabel *spriteLabel;
    QTableWidget *statsTable;
    QSplitter *mainSplitter;
    std::vector<Region> regions;
};

#endif