#include "mainwindow.h"

#include "ui/checkerboard/checkerboardwidget.h"

#include <QAction>
#include <QApplication>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QProcess>
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent),
    m_battle(new Battle(this)),
    m_checkerBoardWidget(new CheckerBoardWidget(this)),
    m_gameMenu(menuBar()->addMenu("Game")),
    m_startGame(new QAction("Start new game", this)),
    m_stopGame(new QAction("End game", this)),
    m_exitGame(new QAction("Quit", this)),
    m_newGameDialog(new NewGameDialog(this))
{
    QApplication::setFont(QFont("u001"));
    // Stylesheet
    QFile stylesheetFile(":/eink.qss");
    stylesheetFile.open(QFile::ReadOnly);
    this->setStyleSheet(stylesheetFile.readAll());
    stylesheetFile.close();
  // 中心控件
  setCentralWidget(m_checkerBoardWidget);

  initMenuBar();
  initSignalsAndSlots();

  m_newGameDialog->setBattle(m_battle);

  m_battle->endBattle(); // 首次重置
}

MainWindow::~MainWindow()
{
}

void MainWindow::initMenuBar()
{
  // 游戏菜单
  m_gameMenu->addAction(m_startGame);
  m_gameMenu->addAction(m_stopGame);
  m_gameMenu->addSeparator();
  m_gameMenu->addAction(m_exitGame);
}

void MainWindow::initSignalsAndSlots()
{
  // menu actions
  connect(m_exitGame, &QAction::triggered, this, []() { QProcess process; process.startDetached("/mnt/onboard/.adds/inkbox/inkbox", QStringList()); qApp->quit(); });
  connect(m_startGame, &QAction::triggered, this, &MainWindow::startNewGame);
  connect(m_stopGame, &QAction::triggered, this, &MainWindow::stopGame);

  // battle and checkerboardwidget
  connect(m_battle, &Battle::sigCheckerBoardChanged, m_checkerBoardWidget,
          &CheckerBoardWidget::setCheckerBoard);
  connect(m_checkerBoardWidget, &CheckerBoardWidget::sigMoved, m_battle, &Battle::slotHumanMove);

  // battle and mainwindow
  connect(m_battle, &Battle::sigBattleEnded, this, &MainWindow::showWinnerInfo);
  connect(m_battle, &Battle::sigChanged, this, &MainWindow::battleChangedHandler);
}

void MainWindow::startNewGame()
{
  m_newGameDialog->show();
}

void MainWindow::stopGame()
{
  m_battle->endBattle();
}

void MainWindow::showWinnerInfo(GamePlay::PlayerColor winner)
{
  QMessageBox *mb = new QMessageBox(this);
  mb->setAttribute(Qt::WA_DeleteOnClose, true);
  mb->setWindowTitle(" ");
  if (winner == GamePlay::Unknown)
    mb->setText("Tie!");
  else
    mb->setText(m_battle->getPlayer(winner)->getName() + " wins!");
  mb->show();
}

void MainWindow::battleChangedHandler()
{
  if (m_battle->isBattleRunning())
  {
    m_startGame->setEnabled(false);
    m_stopGame->setEnabled(true);
    QString currentPlayerName = m_battle->getPlayer(m_battle->currentPlayerColor())->getName();
    statusBar()->showMessage(currentPlayerName + "'s turn", 0);
  }
  else
  {
    m_startGame->setEnabled(true);
    m_stopGame->setEnabled(false);
    statusBar()->showMessage("Start a new game", 0);
  }
}
