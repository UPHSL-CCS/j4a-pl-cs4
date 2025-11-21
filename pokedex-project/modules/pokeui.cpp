#include "pokeui.h"
#include "pokeapi.h"
#include "pokedata.h"

#include <QApplication>
#include <QMessageBox>
#include <QHeaderView>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QPixmap>
#include <QFutureWatcher>
#include <QtConcurrent>

// Constructor
PokedexUI::PokedexUI(QWidget *parent) : QMainWindow(parent) {
    setupUI();
    setWindowTitle("Pokedex v1.0");
    setFixedSize(1000, 800);
    setStyleSheet("background-color: #f0f0f0; font-family: Arial;");
    loadRegions();
}

PokedexUI::~PokedexUI() {}

void PokedexUI::setupUI() {
    QWidget *centralWidget = new QWidget;
    setCentralWidget(centralWidget);

    mainSplitter = new QSplitter(Qt::Horizontal, centralWidget);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    layout->addWidget(mainSplitter);

    // Left side: Regions tabs
    regionTabs = new QTabWidget;
    mainSplitter->addWidget(regionTabs);
    
    // Right side: Details
    QWidget *rightWidget = new QWidget;
    QVBoxLayout *rightLayout = new QVBoxLayout(rightWidget);

    // Search section
    QHBoxLayout *searchLayout = new QHBoxLayout;
    searchInput = new QLineEdit;
    searchInput->setPlaceholderText("Enter Pokemon name or ID");
    searchButton = new QPushButton("Search");
    connect(searchButton, &QPushButton::clicked, this, &PokedexUI::onSearchClicked);
    searchLayout->addWidget(searchInput);
    searchLayout->addWidget(searchButton);
    rightLayout->addLayout(searchLayout);

    listButton = new QPushButton("List First 10 Pokemon");
    connect(listButton, &QPushButton::clicked, this, &PokedexUI::onListClicked);
    rightLayout->addWidget(listButton);

    // Display section
    nameLabel = new QLabel("Name: ");
    idLabel = new QLabel("ID: ");
    typesLabel = new QLabel("Types: ");
    heightLabel = new QLabel("Height: ");
    weightLabel = new QLabel("Weight: ");
    descriptionLabel = new QLabel("Description: ");
    descriptionLabel->setWordWrap(true);
    spriteLabel = new QLabel;
    spriteLabel->setFixedSize(200, 200);
    spriteLabel->setStyleSheet("border: 1px solid #ccc;");

    statsTable = new QTableWidget;
    statsTable->setColumnCount(2);
    statsTable->setHorizontalHeaderLabels({"Stat", "Base Value"});
    statsTable->horizontalHeader()->setStretchLastSection(true);

    rightLayout->addWidget(nameLabel);
    rightLayout->addWidget(idLabel);
    rightLayout->addWidget(typesLabel);
    rightLayout->addWidget(heightLabel);
    rightLayout->addWidget(weightLabel);
    rightLayout->addWidget(descriptionLabel);
    rightLayout->addWidget(spriteLabel);
    rightLayout->addWidget(statsTable);

    mainSplitter->addWidget(rightWidget);
    mainSplitter->setSizes({300, 700});
}

void PokedexUI::loadRegions() {
    QFuture<std::vector<Region>> future = QtConcurrent::run([]() -> std::vector<Region> {
        return PokeAPI::fetchRegions().get(); 
    });

    QFutureWatcher<std::vector<Region>> *watcher = new QFutureWatcher<std::vector<Region>>(this);
    connect(watcher, &QFutureWatcher<std::vector<Region>>::finished, this, [this, watcher]() {
        regions = watcher->result();
        for (const auto& reg : regions) {
            QListWidget *list = new QListWidget;
            list->addItem("Select this region to load Pokémon...");
            connect(list, &QListWidget::itemClicked, this, &PokedexUI::onPokemonSelected);
            regionTabs->addTab(list, QString::fromStdString(reg.name));
        }
        connect(regionTabs, &QTabWidget::currentChanged, this, &PokedexUI::onRegionTabChanged);
        
        // Auto-load first region
        if (!regions.empty()) {
            onRegionTabChanged(0);
        }
        
        watcher->deleteLater();
    });
    watcher->setFuture(future);
}
  
void PokedexUI::onRegionTabChanged(int index) {
    if (index < 0 || index >= (int)regions.size()) return;
    
    QListWidget *currentList = qobject_cast<QListWidget*>(regionTabs->widget(index));
    if (!currentList) return;
    
    // Check if already loaded (more than just the placeholder)
    if (currentList->count() > 1) return;
    
    // Clear placeholder
    currentList->clear();
    currentList->addItem("Loading...");
    
    const std::string& regionName = regions[index].name;
    
    // Fetch Pokémon for this region asynchronously
    QFuture<std::vector<std::string>> future = QtConcurrent::run([regionName]() {
        return PokeAPI::fetchPokemonByRegion(regionName).get();
    });
    
    QFutureWatcher<std::vector<std::string>> *watcher = new QFutureWatcher<std::vector<std::string>>(this);
    connect(watcher, &QFutureWatcher<std::vector<std::string>>::finished, this, [this, watcher, currentList, index]() {
        std::vector<std::string> pokemonNames = watcher->result();
        
        currentList->clear();
        if (pokemonNames.empty()) {
            currentList->addItem("No Pokémon found for this region");
        } else {
            for (const auto& name : pokemonNames) {
                currentList->addItem(QString::fromStdString(name));
            }
        }
        
        // Store in regions data
        if (index >= 0 && index < (int)regions.size()) {
            regions[index].pokemon_names = pokemonNames;
        }
        
        watcher->deleteLater();
    });
    watcher->setFuture(future);
}

void PokedexUI::onPokemonSelected(QListWidgetItem *item) {
    // Prevent fetching if clicking the placeholder text
    QString text = item->text();
    if(text != "Select a region to load a pokemon..." &&
       text != "Loading..." &&
       text != "No Pokémon found for this region") {
        fetchAndDisplay(text);
    }
}

void PokedexUI::onSearchClicked() {
    QString query = searchInput->text().trimmed();
    if (!query.isEmpty()) {
        fetchAndDisplay(query);
    }
}

void PokedexUI::onListClicked() {
    // Fetches first 10 pokemon just by iterating IDs
    for(int i=1; i<=10; i++) {
        // Note: Ideally this would populate a list, but for now we just display the last one
        if(i==1) fetchAndDisplay(QString::number(i)); 
    }
}

void PokedexUI::fetchAndDisplay(const QString& query) {
    std::string qStd = query.toStdString();
    QFuture<Pokemon> future = QtConcurrent::run([qStd]() {
        return PokeAPI::fetchPokemonWithDescription(qStd).get();
    });

    QFutureWatcher<Pokemon> *watcher = new QFutureWatcher<Pokemon>(this);
    connect(watcher, &QFutureWatcher<Pokemon>::finished, this, [this, watcher]() {
        try {
            Pokemon p = watcher->result();
            displayPokemon(p);
        } catch (const std::exception& e) {
            displayError(QString::fromStdString(e.what()));
        }
        watcher->deleteLater();
    });
    watcher->setFuture(future);
}

void PokedexUI::displayPokemon(const Pokemon& p) {
    nameLabel->setText("Name: " + QString::fromStdString(p.name));
    idLabel->setText("ID: " + QString::number(p.id));
    
    QString types;
    for (size_t i = 0; i < p.types.size(); ++i) {
        types += QString::fromStdString(p.types[i]);
        if (i < p.types.size() - 1) types += ", ";
    }
    
    typesLabel->setText("Types: " + types);
    heightLabel->setText("Height: " + QString::number(p.height / 10.0) + " m");
    weightLabel->setText("Weight: " + QString::number(p.weight / 10.0) + " kg");
    descriptionLabel->setText("Description: " + QString::fromStdString(p.description));

    // Load sprite
    if (!p.sprite_url.empty()) {
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        QNetworkRequest request(QUrl(QString::fromStdString(p.sprite_url)));
        QNetworkReply *reply = manager->get(request);
        connect(reply, &QNetworkReply::finished, this, [this, reply]() {
            if (reply->error() == QNetworkReply::NoError) {
                QPixmap pixmap;
                pixmap.loadFromData(reply->readAll());
                spriteLabel->setPixmap(pixmap.scaled(200, 200, Qt::KeepAspectRatio));
            }
            reply->deleteLater();
        });
    }

    // Stats table
    statsTable->setRowCount((int)p.stats.size());
    for (size_t i = 0; i < p.stats.size(); ++i) {
        statsTable->setItem((int)i, 0, new QTableWidgetItem(QString::fromStdString(p.stats[i].name)));
        statsTable->setItem((int)i, 1, new QTableWidgetItem(QString::number(p.stats[i].base_stat)));
    }
}

void PokedexUI::displayError(const QString& msg) {
    QMessageBox::critical(this, "Error", msg);
}