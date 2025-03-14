#include <QDir>
#include <iostream>

#include "utils/Logger.h"

Q_LOGGING_CATEGORY(loggerCategory, "Logger")

std::shared_ptr<Logger> Logger::m_instance = nullptr;

void Logger::setInstance(std::shared_ptr<Logger> instance) {
  m_instance = instance;
}

std::shared_ptr<Logger> Logger::getInstance() const { return m_instance; }

void Logger::messageHandler(QtMsgType type, const QMessageLogContext &context,
                            const QString &msg) {
  if (m_instance) {

    switch (type) {
    case QtDebugMsg:
      m_instance->setLogLevel(Logger::LogLevel::Debug);
      break;
    case QtInfoMsg:
      m_instance->setLogLevel(Logger::LogLevel::Info);
      break;
    case QtWarningMsg:
      m_instance->setLogLevel(Logger::LogLevel::Warning);
      break;
    case QtCriticalMsg:
      m_instance->setLogLevel(Logger::LogLevel::Critical);
      break;
    case QtFatalMsg:
      m_instance->setLogLevel(Logger::LogLevel::Fatal);
      break;
    }

    m_instance->log(QString::fromLatin1(context.category), msg);
  }
}

Logger::Logger(QObject *parent)
    : QObject(parent), m_logLevel(LogLevel::Unknown) {
  init();
}

Logger::~Logger() {
  if (m_logFile.isOpen()) {
    m_logFile.close();
  }
}

void Logger::init() {

  auto timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss");
  auto filename = QString("%1_log.txt").arg(timestamp);
  auto logDirectory = QString("logs");

  auto fullFileName = QString("%1/%2").arg(logDirectory, filename);
  m_logFile.setFileName(fullFileName);

  if (!m_logFile.open(QIODevice::Append | QIODevice::Text)) {
    std::cerr << "Failed to open log file: " << fullFileName.toStdString();
  }

  // if (!m_instance)
  //   m_instance = std::make_shared<Logger>(this);

  qInstallMessageHandler(Logger::messageHandler);
}

QString Logger::log(const QString &categoryName, const QString &msg) const {

  auto logMsg = QString();

  logMsg = formatMessage(categoryName, msg);
  writeToFile(logMsg);

  std::cerr << logMsg.toStdString() << '\n';

  return logMsg;
}

void Logger::setLogLevel(LogLevel level) { m_logLevel = level; }

QString Logger::formatMessage(const QString &categoryName,
                              const QString &msg) const {
  auto timestamp = QDateTime::currentDateTime().toString(Qt::ISODate);
  auto levelStr = loglevelToStr(m_logLevel);
  auto logMsg =
      QString("[%1] [%2] [%3]: %4").arg(timestamp, levelStr, categoryName, msg);

  return logMsg;
}

void Logger::writeToFile(const QString &message) const {
  auto locker = QMutexLocker(&m_mutex);

  if (m_logFile.isOpen()) {
    auto stream = QTextStream(&m_logFile);
    stream << message << Qt::endl;
    m_logFile.flush();
  }
}

QString Logger::loglevelToStr(const LogLevel &logLevel) const {
  switch (logLevel) {
  case LogLevel::Debug:
    return QString("Debug");
    break;
  case LogLevel::Info:
    return QString("Info");
    break;
  case LogLevel::Warning:
    return QString("Warning");
    break;
  case LogLevel::Critical:
    return QString("Critical");
    break;
  case LogLevel::Fatal:
    return QString("Fatal");
    break;
  default:
    return QString("Unknown");
    break;
  }
}
