#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils.h"

#include <QSettings>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QDateTime>

QSettings *mySettings;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Load settings.
    mySettings = new QSettings("stevenlafl", "empyrionsave");
    this->loadSettings();
}

MainWindow::~MainWindow() {
    delete mySettings;
    if (timer.isActive()) {
        timer.stop();
    }
    delete ui;
}

void MainWindow::loadSettings() {
    int x = mySettings->value("timerSlider").toInt();

    // Trigger slider move.
    ui->timerSlider->setValue(x);
    this->on_timerSlider_sliderMoved(x);

    ui->backupEdit->setText(mySettings->value("backupFolder").toString());
    ui->saveEdit->setText(mySettings->value("saveFolder").toString());
    ui->saveZipCheck->setChecked(mySettings->value("saveAsZip").toBool());

    ui->statusbar->showMessage("Settings loaded", 1 * 1000);
}

void MainWindow::saveSettings() {
    mySettings->setValue("timerSlider", ui->timerSlider->value());
    mySettings->setValue("backupFolder", ui->backupEdit->text());
    mySettings->setValue("saveFolder", ui->saveEdit->text());
    mySettings->setValue("saveAsZip", ui->saveZipCheck->isChecked());

    ui->statusbar->showMessage("Settings saved", 1 * 1000);
}

void MainWindow::backupSave() {

    QString backupDir = ui->backupEdit->text();
    QString saveDir = ui->saveEdit->text();

    QString timeString = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss");
    QString fileName =  QDir(saveDir).dirName() + "_" + timeString;
    QString fullPath = backupDir + "/" + fileName;

    if (ui->saveZipCheck->isChecked()) {
        fullPath += ".zip";
        fileName += ".zip";
        if (Utils::archive(fullPath, saveDir)) {
            ui->statusbar->showMessage("Backup saved: " + fileName, 1 * 1000);
        }
        else {
            ui->statusbar->showMessage("Backup failed: " + fileName, 1 * 1000);
        }
    }
    else {
        if (Utils::copyPath(saveDir, fullPath, true)) {
            ui->statusbar->showMessage("Backup saved: " + fileName, 1 * 1000);
        }
        else {
            ui->statusbar->showMessage("Backup failed: " + fileName, 1 * 1000);
        }
    }

    ui->textEdit->setText(saveDir + " ---> " + fullPath);
}

void MainWindow::closeEvent(QCloseEvent *event) {
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Empyrion Save",
                                                                "Are you sure you want to exit?",
                                                                QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
                                                                QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        event->accept();
    }
}

void MainWindow::timerEvent(QTimerEvent *event) {
    if (event->timerId() == timer.timerId()) {
        timerCounter++;

        if (timerCounter > timerMax) {
            timerCounter = 0;
            this->backupSave();
        }
        else {
            ui->statusbar->showMessage("Last saved " + this->getMinuteString(timerCounter) + " ago", 1 * 1000);
        }
    } else {
        QWidget::timerEvent(event);
    }
}

QString MainWindow::getMinuteString(int seconds) {
    int min = seconds / 60;
    int sec = seconds % 60;
    QString time = "";

    time += QString("%1m%2s").arg(
                QVariant(min).toString().rightJustified(2, '0'),
                QVariant(sec).toString().rightJustified(2, '0'));

    return time;
}

void MainWindow::on_timerSlider_sliderMoved(int position) {
    ui->intervalLabel->setText(this->getMinuteString(position));
}

void MainWindow::on_saveBrowse_clicked() {
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    ui->saveEdit->text(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty())
    {
        ui->saveEdit->setText(dir);
    }
}

void MainWindow::on_backupBrowse_clicked() {
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    ui->backupEdit->text(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty()) {
        ui->backupEdit->setText(dir);
    }
}

void MainWindow::on_saveButton_clicked() {
    this->saveSettings();
}

void MainWindow::on_startButton_clicked() {
    if (!timer.isActive()) {
        timerCounter = 0;
        timerMax = ui->timerSlider->value();
        timer.start(1 * 1000, this);
        ui->startButton->setText("Stop");
    }
    else {
        timerCounter = 0;
        ui->startButton->setText("Start");
        timer.stop();
    }
}
