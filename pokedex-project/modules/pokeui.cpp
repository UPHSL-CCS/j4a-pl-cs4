#include "pokeui.h"
#include "pokeapi.h"  // Your API header
#include <QApplication>
#include <QMessageBox>
#include <QHeaderView>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QPixmap>
#include <QFutureWatcher>
#include <QtConcurrent>

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
    mainSplitter->setSizes({300, 700});

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
}

  void PokedexUI::loadRegions() {
      std::future<std::vector<Region>> future = PokeAPI::fetchRegions();  // Change to std::future
      QFutureWatcher<std::vector<Region>> *watcher = new QFutureWatcher<std::vector<Region>>(this);
      connect(watcher, &QFutureWatcher<std::vector<Region>>::finished, this, [this, watcher]() {
          regions = watcher->result();
          for (const auto& reg : regions) {
              QListWidget *list = new QListWidget;
              // Note: Your fetchRegions doesn't populate pokemon_names yet.
              // For now, add a placeholder or fetch separately.
              list->addItem("Pokemon list not loaded yet");  // Update when you add pokemon fetching per region
              connect(list, &QListWidget::itemClicked, this, &PokedexUI::onPokemonSelected);
              regionTabs->addTab(list, QString::fromStdString(reg.name));
          }
          connect(regionTabs, &QTabWidget::currentChanged, this, &PokedexUI::onRegionTabChanged);
          watcher->deleteLater();
      });
      watcher->setFuture(future);  // This works with std::future
  }

void PokedexUI::onRegionTabChanged(int index) {
    // Optional: Load details for the first Pokemon in the tab
}

void PokedexUI::onPokemonSelected(QListWidgetItem *item) {
    fetchAndDisplay(item->text());
}

void PokedexUI::onSearchClicked() {
    QString query = searchInput->text().trimmed();
    if (!query.isEmpty()) {
        fetchAndDisplay(query);
    }
}

void PokedexUI::onListClicked() {
    // Similar to before, but could integrate with tabs
}

void PokedexUI::fetchAndDisplay(const QString& query) {
    QFuture<Pokemon> future = QtConcurrent::run([query]() {
        return PokeAPI::fetchPokemonWithDescription(query.toStdString()).get();
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
    statsTable->setRowCount(p.stats.size());
    for (int i = 0; i < p.stats.size(); ++i) {
        statsTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(p.stats[i].name)));
        statsTable->setItem(i, 1, new QTableWidgetItem(QString::number(p.stats[i].base_stat)));
    }
}

void PokedexUI::displayError(const QString& msg) {
    QMessageBox::critical(this, "Error", msg);
}