#include "AccountWindow.hxx"

#include "CSVModel.hxx"
#include "MonthlyCSVGenerator.hxx"

#include <QTableView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QSortFilterProxyModel>
#include <QDate>

#include <QDebug>

AccountWindow::AccountWindow(QWidget* parent):
  QWidget(parent) {

  QDate currentDate = QDate::currentDate();
  currentDate = currentDate.addMonths(-1);
  m_month = currentDate.month();
  m_year = currentDate.year();

  QFont font;
  font.setPixelSize(25);
  font.setCapitalization(QFont::Capitalize);
  font.setBold(99);
  QFontMetrics fm(font);
  m_monthLabel = new QLabel(currentDate.toString("MMMM"));
  m_monthLabel->setFont(font);
  m_monthLabel->setFixedWidth(fm.width(QDate(1, 9, 1).toString("MMMM")));
  m_monthLabel->setAlignment(Qt::AlignCenter);
  m_yearLabel = new QLabel(currentDate.toString("yyyy"));
  m_yearLabel->setFont(font);
  m_yearLabel->setFixedWidth(fm.width(m_yearLabel->text()));
  auto dateLabelLayout = new QHBoxLayout;
  dateLabelLayout->addWidget(m_monthLabel);
  dateLabelLayout->addWidget(m_yearLabel);
  m_previousYear = new QPushButton("<<");
  m_previousYear->setFont(font);
  m_previousYear->setFixedSize(m_previousYear->sizeHint().height(), m_previousYear->sizeHint().height());
  m_previousMonth = new QPushButton("<");
  m_previousMonth->setFont(font);
  m_previousMonth->setFixedSize(m_previousMonth->sizeHint().height(), m_previousMonth->sizeHint().height());
  m_nextMonth = new QPushButton(">");
  m_nextMonth->setFont(font);
  m_nextMonth->setFixedSize(m_nextMonth->sizeHint().height(), m_nextMonth->sizeHint().height());
  m_nextYear = new QPushButton(">>");
  m_nextYear->setFont(font);
  m_nextYear->setFixedSize(m_nextYear->sizeHint().height(), m_nextYear->sizeHint().height());

  connect(m_previousYear, SIGNAL(clicked()), this, SLOT(goToPreviousYear()));
  connect(m_previousMonth, SIGNAL(clicked()), this, SLOT(goToPreviousMonth()));
  connect(m_nextMonth, SIGNAL(clicked()), this, SLOT(goToNextMonth()));
  connect(m_nextYear, SIGNAL(clicked()), this, SLOT(goToNextYear()));
  connect(this, SIGNAL(yearChanged()), this, SLOT(updateYear()));
  connect(this, SIGNAL(monthChanged()), this, SLOT(updateMonth()));
  connect(this, SIGNAL(updateModelRequested()), this, SLOT(fillModel()));

  auto dateLayout = new QHBoxLayout;
  dateLayout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Preferred));
  dateLayout->addWidget(m_previousYear);
  dateLayout->setAlignment(m_previousYear, Qt::AlignCenter);
  dateLayout->addWidget(m_previousMonth);
  dateLayout->setAlignment(m_previousMonth, Qt::AlignCenter);
  dateLayout->addLayout(dateLabelLayout);
  dateLayout->setAlignment(dateLabelLayout, Qt::AlignCenter);
  dateLayout->addWidget(m_nextMonth);
  dateLayout->setAlignment(m_nextMonth, Qt::AlignCenter);
  dateLayout->addWidget(m_nextYear);
  dateLayout->setAlignment(m_nextYear, Qt::AlignCenter);
  dateLayout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Preferred));

  m_csvModel = new CSVModel(this);
  fillModel();

  auto proxyModel = new QSortFilterProxyModel(this);
  proxyModel->setSourceModel(m_csvModel);

  m_tableView = new QTableView;
  m_tableView->setModel(proxyModel);
  m_tableView->setAlternatingRowColors(true);
  m_tableView->verticalHeader()->hide();
  if (m_csvModel->rowCount() > 0) {
    m_tableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
  }
  m_tableView->setFocusPolicy(Qt::NoFocus);
  m_tableView->setSelectionMode(QTableView::NoSelection);
  //m_tableView->setSortingEnabled(true);
  //m_tableView->sortByColumn(0, Qt::AscendingOrder);

  auto mainLayout = new QVBoxLayout;
  mainLayout->addLayout(dateLayout);
  mainLayout->addWidget(m_tableView);

  setLayout(mainLayout);
}

void AccountWindow::goToPreviousYear() {
  m_year--;
  emit yearChanged();
  emit updateModelRequested();
}

void AccountWindow::goToPreviousMonth() {
  //m_month==1?m_month=12,m_year--:m_month=(m_month+=11)%12;
  if (m_month == 1) {
    m_month = 12;
    m_year--;
    emit yearChanged();
  } else
    m_month--;

  emit monthChanged();
  emit updateModelRequested();
}

void AccountWindow::goToNextMonth() {
  //m_month=(m_month+=13)%12==0?m_month=12:m_year+=(m_month==1);
  if (m_month == 12) {
    m_month = 1;
    m_year++;
    emit yearChanged();
  } else
    m_month++;

  emit monthChanged();
  emit updateModelRequested();
}

void AccountWindow::goToNextYear() {
  m_year++;
  emit yearChanged();
  emit updateModelRequested();
}

void AccountWindow::updateMonth() {
  m_monthLabel->setText(QDate(1, m_month, 1).toString("MMMM"));
}

void AccountWindow::updateYear() {
  m_yearLabel->setText(QString::number(m_year));
}

void AccountWindow::fillModel() {
  //MonthlyCSVGenerator::convertRawCSVToMonthlyCSV(QDate::currentDate(), "../BankAccount/csv/raw.csv");
  m_csvModel->setSource("../BankAccount/csv/"+QDate(m_year, m_month, 1).toString("MM-yyyy")+"/operations.csv");
}
