#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

#include "SensorRepository.h"

Q_LOGGING_CATEGORY(SensorRepositoryCategory, "SensorRepository")

SensorRepository::SensorRepository(QObject *parent)
    : QObject(parent), m_database(), m_mutex(), m_connectionName() {
  m_connectionName = QString(QUuid::createUuid().toString());
}

SensorRepository::~SensorRepository() {
  m_database.isOpen() ? m_database.close()
                      : QSqlDatabase::removeDatabase(m_connectionName);
}

bool SensorRepository::init(const QString &databasePath) {
  auto locker = QMutexLocker(&m_mutex);

  m_database = QSqlDatabase::addDatabase("QSQLITE", m_connectionName);
  m_database.setDatabaseName(databasePath);

  if (!m_database.open()) {
    qCCritical(SensorRepositoryCategory)
        << QString("Failed to open database: %1")
               .arg(m_database.lastError().text());

    emit databaseError(m_database.lastError().text());

    return false;
  }

  if (!createTables()) {
    qCCritical(SensorRepositoryCategory) << QString("Failed to create tables");
    m_database.close();
    QSqlDatabase::removeDatabase(m_connectionName);
    return false;
  }

  qCInfo(SensorRepositoryCategory)
      << QString("Database initialized successfully");
  return true;
}

bool SensorRepository::saveSensorData(const SensorData &data) {
  auto locker = QMutexLocker(&m_mutex);

  if (!data.isValid()) {
    qCWarning(SensorRepositoryCategory)
        << QString(" attempt to save invalid data");
    return false;
  }

  auto query = QSqlQuery(m_database);
  auto queryString =
      QString("INSERT INTO sensor_data (timestamp, temperature, humidity) "
              "VALUES (:timestamp, :temperature, :humidity)");

  query.prepare(queryString);
  query.bindValue(":timestamp", data.timestamp().toString(Qt::ISODate));
  query.bindValue(":temperature", data.getTemp());
  query.bindValue(":humidity", data.getHumidity());

  if (query.exec()) {
    qCWarning(SensorRepositoryCategory)
        << QString("Failed to save data: %1").arg(query.lastError().text());

    emit databaseError(query.lastError().text());

    return false;
  }

  return true;
}

QVector<SensorData> SensorRepository::getHistoricalData(const QDateTime &from,
                                                        const QDateTime &to,
                                                        int maxResults) {
  auto locker = QMutexLocker(&m_mutex);
  auto query = QSqlQuery(m_database);
  auto data = QVector<SensorData>();
  auto queryString = QString("SELECT timestamp, temperature, humidity "
                             "FROM sensor_data "
                             "WHERE timestamp BETWEEN :from AND :to "
                             "ORDER BY timestamp DESC "
                             "LIMIT :limit");

  query.prepare(queryString);
  query.bindValue(":to", to.toString(Qt::ISODate));
  query.bindValue(":from", from.toString(Qt::ISODate));
  query.bindValue(":limit", maxResults);

  if (!query.exec()) {
    qCWarning(SensorRepositoryCategory)
        << QString("Failed to get historical data: %1")
               .arg(query.lastError().text());

    emit databaseError(query.lastError().text());

    return data;
  }

  while (query.next()) {
    auto timestamp = QDateTime::fromString(
        query.value("timestamp").toDateTime().toString(), Qt::ISODate);
    auto temp = query.value("temperature").toDouble();
    auto humidity = query.value("humidity").toDouble();

    auto sensorData = SensorData(temp, humidity, timestamp);

    if (sensorData.isValid())
      data.append(sensorData);
  }

  return data;
}

bool SensorRepository::purgeOldData(int daysToKeep) {
  auto locker = QMutexLocker(&m_mutex);
  auto cutoff = QDateTime::currentDateTime().addDays(-daysToKeep);
  auto query = QSqlQuery(m_database);
  auto queryString =
      QString("DELETE FROM sensor_data WHERE timestamp < :cutoff");

  query.prepare(queryString);
  query.bindValue(":cutoff", cutoff.toString(Qt::ISODate));

  if (!query.exec()) {
    qCCritical(SensorRepositoryCategory)
        << QString("Failed to purge data: %1").arg(query.lastError().text());

    emit databaseError(query.lastError().text());

    return false;
  }

  qCInfo(SensorRepositoryCategory)
      << QString("Purged data older than %1 days").arg(daysToKeep);
  return true;
}

bool SensorRepository::createTables() {
  auto queryString = QString("CREATE TABLE IF NOT EXISTS sensor_data ("
                             "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                             "timestamp DATETIME NOT NULL, "
                             "temperature REAL NOT NULL, "
                             "humidity REAL NOT NULL, "
                             "CHECK(temperature BETWEEN -50 AND 100)"
                             "CHECK(humidity BETWEEN 0 AND 100)"
                             ")");

  return executeQuery(queryString);
}

bool SensorRepository::executeQuery(const QString &queryStr) {
  auto locker = QMutexLocker(&m_mutex);
  auto query = QSqlQuery(m_database);

  if (!query.exec(queryStr)) {
    qCCritical(SensorRepositoryCategory())
        << QString("Failed to execute query: %1").arg(query.lastError().text());
    emit databaseError(query.lastError().text());
    return false;
  }

  return true;
}
