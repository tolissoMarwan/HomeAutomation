#pragma once

#include <QMainWindow>
#include <QtCharts/QChartView>
#include <QtWidgets/QTableWidget>

#include "DeviceManager.h"
#include "SensorRepository.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  explicit MainWindow(std::shared_ptr<DeviceManager> deviceManager,
                      std::shared_ptr<SensorRepository> sensorRepo,
                      QWidget *parent = nullptr);
  ~MainWindow();

public slots:
  void updateSensorReadings(const SensorData &data);
  void updateDeviceReadings(const DeviceState &state);
  void showTemperatureAlert(const QString &message);
  void showHumidityAlert(const QString &message);

private slots:
  void onLightToggleClicked();
  void onBrightnessSliderChanged(int value);
  void refreshHistoricalData();

private:
  void setupUI();
  void setupConnections();
  void initCharts();

  void setChart(const QString &title);

  std::shared_ptr<Ui::MainWindow> ui;
  std::shared_ptr<DeviceManager> m_deviceManger;
  std::shared_ptr<SensorRepository> m_sensorRepo;

  std::unique_ptr<QChart> m_temperatureChart;
  std::unique_ptr<QChart> m_humidityChart;

  std::shared_ptr<QTableWidget> m_logTable;
};
