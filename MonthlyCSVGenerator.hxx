#ifndef MONTHLYCSVGENERATOR_HXX
#define MONTHLYCSVGENERATOR_HXX

#include <exception>
#include <string>

#include <QChar>
#include <QDate>

class QString;

class MonthlyCSVGenerator {
public:
  MonthlyCSVGenerator();

  static void ConvertRawCSVToMonthlyCSV(QDate const& p_date, QChar const& p_delim = ';',
    QString const& p_outputFileName = "operations.csv", QString const& p_rawFileName = "raw.csv");
  static void SaveCategory(int p_row, QString const& p_group, QString const& p_category, QString const& p_inFileName);
  static void UpdateRawCSV(QDate const& p_date, QString const& p_inFileName, QChar const& p_delim = ';', bool p_hasHeader = true);
  static QString ConvertXLSToCSV(QString const& p_xlsFileName);

protected:
  static void CleanOperations(QString const& p_fileName);

private:
  static QString GetOperationType(QString const& p_operationType);
};

class OpenFailure: public std::exception {
public:
  OpenFailure(std::string const& fileName, bool p_readMode = true):
    m_fileName(fileName),
    m_readMode(p_readMode) {
  }

private:
  virtual const char* what() const throw() {
    auto message = std::string("Cannot open "+m_fileName+" in "+(m_readMode?"read":"write")+" mode.");
    return message.c_str();
  }

  std::string m_fileName;
  bool m_readMode;
};

#endif
