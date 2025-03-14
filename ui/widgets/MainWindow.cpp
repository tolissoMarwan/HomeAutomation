#include <QMessageBox>
#include <QtCharts/QLineSeries>

#include "widgets/MainWindow.h"
#include "widgets/ui_MainWindow.h"

MainWindow::MainWindow(std::shared_ptr<DeviceManager> deviceManager,
                       std::shared_ptr<SensorRepository> sensorRepo,
                       QWidget *parent)
    : QMainWindow(parent), ui(std::make_shared<Ui::MainWindow>()),
      m_deviceManger(deviceManager), m_sensorRepo(sensorRepo),
      m_temperatureChart(std::make_unique<QChart>()),
      m_humidityChart(std::make_unique<QChart>()),
      m_logTable(std::make_shared<QTableWidget>(this)) {
  ui->setupUi(this);
  setupUI();
  setupConnections();
  initCharts();
}

MainWindow::~MainWindow(){};

void MainWindow::setupUI() {
  ui->logTable->setColumnCount(3);
  ui->logTable->setHorizontalHeaderLabels({"Time", "Type", "Value"});

  auto tempChartView = std::make_unique<QChartView>(m_temperatureChart.get());
  auto humidityChartView = std::make_unique<QChartView>(m_humidityChart.get());

  ui->chartLayout->addWidget(tempChartView.release());
  ui->chartLayout->addWidget(humidityChartView.release());

  ui->brightnessSlider->setRange(0, 100);
  ui->brightnessSlider->setValue(0);
}

void MainWindow::setupConnections() {
  connect(ui->lightToggleButton, &QPushButton::clicked, this,
          &MainWindow::onLightToggleClicked);
  connect(ui->brightnessSlider, &QSlider::valueChanged, this,
          &MainWindow::onBrightnessSliderChanged);

  connect(ui->refreshButton, &QPushButton::clicked, this,
          &MainWindow::refreshHistoricalData);
}

void MainWindow::onBrightnessSliderChanged(int value) {
  m_deviceManger->setLightBrightness(1, value);
}

// MainWindow::setChart(const QString &title, std::shared_ptr<T> chart) {}

void MainWindow::initCharts() {
  m_temperatureChart->setTitle("Temperature History");
  auto tempSeries = std::make_shared<QLineSeries>();
  m_temperatureChart->addSeries(tempSeries.get());
  m_temperatureChart->createDefaultAxes();

  m_humidityChart->setTitle("Humidity History");
  auto humidSeries = std::make_shared<QLineSeries>();
  m_humidityChart->addSeries(humidSeries.get());
  m_humidityChart->createDefaultAxes();
}

void MainWindow::updateSensorReadings(const SensorData &data) {
  ui->tempLabel->setText(QString::number(data.getTemp()) + " °C");
  ui->humidityLabel->setText(QString::number(data.getHumidity()) + " %");

  auto tempSeries =
      qobject_cast<QLineSeries *>(m_temperatureChart->series().at(0));
  tempSeries->append(QDateTime::currentMSecsSinceEpoch(), data.getTemp());

  auto humidSeries =
      qobject_cast<QLineSeries *>(m_humidityChart->series().at(0));
  humidSeries->append(QDateTime::currentMSecsSinceEpoch(), data.getHumidity());
}

void MainWindow::updateDeviceReadings(const DeviceState &state) {
  if ("type" == state.type()) {
    ui->lightToggleButton->setText(state.light() ? "Turn Off" : "Turn On");
    ui->brightnessSlider->setValue(state.brightness());
  }

  const int row = m_logTable->rowCount();
  m_logTable->insertRow(row);
  m_logTable->setItem(
      row, 0, new QTableWidgetItem(QDateTime::currentDateTime().toString()));
  m_logTable->setItem(row, 0, new QTableWidgetItem(state.type()));
  m_logTable->setItem(row, 0,
                      new QTableWidgetItem(state.light() ? "ON" : "OFF"));
}

void MainWindow::onLightToggleClicked() {
  const bool currentState = m_deviceManger->isLightOn(1);
  m_deviceManger->toggleLight(1, !currentState);
}

void MainWindow::refreshHistoricalData() {
  const auto history = m_sensorRepo->getHistoricalData(
      QDateTime::currentDateTime().addDays(-1), QDateTime::currentDateTime());

  auto tempSeries =
      qobject_cast<QLineSeries *>(m_temperatureChart->series().at(0));
  tempSeries->clear();

  for (const auto &data : history) {
    tempSeries->append(data.timestamp().toMSecsSinceEpoch(), data.getTemp());
  }

  // Calculate min/max from series data
  double minX = 0, maxX = 0, minY = 0, maxY = 0;
  for (int i = 0; i < tempSeries->points().size(); ++i) {
    const auto point = tempSeries->points().at(i);
    minX = qMin(minX, point.x());
    maxX = qMax(maxX, point.x());
    minY = qMin(minY, point.y());
    maxY = qMax(maxY, point.y());
  }

  // Set axis range
  m_temperatureChart->axes(Qt::Horizontal).first()->setRange(minX, maxX);
  m_temperatureChart->axes(Qt::Vertical).first()->setRange(minY, maxY);
}

void MainWindow::showTemperatureAlert(const QString &message) {
  QMessageBox::warning(this, "Temperature Alert", message);
  ui->statusbar->showMessage(message, 5000);
}

void MainWindow::showHumidityAlert(const QString &message) {
  QMessageBox::warning(this, "Humidity Alert", message);
  ui->statusbar->showMessage(message, 5000);
}
