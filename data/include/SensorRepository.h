#include <QObject>
#include <QVector>
#include <QtSql/QSqlDatabase>

#include "dtos/SensorData.h"
#include "utils/Logger.h"

Q_DECLARE_LOGGING_CATEGORY(SensorRepositoryCategory)

class SensorRepository : public QObject {
  Q_OBJECT
public:
  explicit SensorRepository(QObject *parent = nullptr);
  ~SensorRepository();

  bool init(const QString &databasePath = "sensors.db");
  bool saveSensorData(const SensorData &data);
  QVector<SensorData> getHistoricalData(const QDateTime &from,
                                        const QDateTime &to,
                                        int maxResults = 1000);
  bool purgeOldData(int daysToKeep = 30);

signals:
  void databaseError(const QString &error);

private:
  bool createTables();
  bool executeQuery(const QString &queryStr);

  QSqlDatabase m_database;
  QMutex m_mutex;
  QString m_connectionName;
};
