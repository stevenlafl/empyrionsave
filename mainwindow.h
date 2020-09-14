#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QBasicTimer>
#include <QCloseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void closeEvent(QCloseEvent (*event));
    QString getMinuteString(int seconds);

private slots:
    void on_timerSlider_sliderMoved(int position);

    void on_saveBrowse_clicked();

    void on_backupBrowse_clicked();

    void on_saveButton_clicked();

    void on_startButton_clicked();

private:
    Ui::MainWindow *ui;
    QBasicTimer timer;
    int timerCounter = 0;
    int timerMax = 0;

protected:
    void timerEvent(QTimerEvent *event) override;
    void loadSettings();
    void saveSettings();

    void backupSave();
};
#endif // MAINWINDOW_H
