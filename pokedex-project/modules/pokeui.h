#pragma once

#include "pokedata.h" 
#include "pokeapi.h" 

#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QTabWidget>
#include <QListWidget>
#include <QSplitter>
#include <QPixmap>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QApplication> 
#include <QtConcurrent> 
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <map>

// FIXED: Renamed class from PokedexWindow to PokedexUI
class PokedexUI : public QMainWindow {
    Q_OBJECT 

public:
    // Constructor matches your implementation (no apiService arg needed)
    explicit PokedexUI(QWidget *parent = nullptr);
    ~PokedexUI();

private slots:
    void onSearchClicked();
    void onPokemonSelected(QListWidgetItem *item);
    void onRegionTabChanged(int index);

private:
    void setupUI();
    void loadRegions();
    void fetchAndDisplay(const QString& query);
    void displayPokemon(const Pokemon& p);
    void displayError(const QString& msg);
    std::map<int, std::string> pokedexUrls; // Store pokedex URLs by region index

    // UI Components matching your .cpp
    QSplitter *mainSplitter;
    QTabWidget *regionTabs;
    QLineEdit *searchInput;
    QPushButton *searchButton;
    
    QLabel *nameLabel;
    QLabel *idLabel;
    QLabel *typesLabel;
    QLabel *heightLabel;
    QLabel *weightLabel;
    QLabel *descriptionLabel;
    QLabel *spriteLabel;
    
    QTableWidget *statsTable;

    // Data storage
    std::vector<Region> regions;
};