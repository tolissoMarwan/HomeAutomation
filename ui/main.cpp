#include "widgets/MainWindow.h"

#include <QApplication>

Q_LOGGING_CATEGORY(mainCategory, "MAIN")

int main(int argc, char *argv[]) {
  auto a = QApplication(argc, argv);

  auto logger = std::make_shared<Logger>();
  Logger::setInstance(logger);

  auto mqttService = std::make_shared<MqttService>();
  auto deviceManager = std::make_shared<DeviceManager>(mqttService);
  auto sensorRepo = std::make_shared<SensorRepository>();

  mqttService->connectToBroker("broker.hivemq.com", 1883);
  deviceManager->registerDevice(1, "light");
  deviceManager->registerDevice(2, "thermostat");

  QFile styleFile(":/style/darktheme.qss");

  if (styleFile.open(QIODevice::ReadOnly)) {
    qApp->setStyleSheet(styleFile.readAll());
  } else {
    qCCritical(mainCategory) << "Failed to load stylesheet";
  }

  auto mainWindow = MainWindow(deviceManager, sensorRepo);
  mainWindow.show();

  QObject::connect(&mainWindow, &MainWindow::destroyed, [&]() {
    deviceManager->unregisterDevice(1);
    deviceManager->unregisterDevice(2);
    mqttService->disconnectFromBroker();
  });

  return a.exec();
}
