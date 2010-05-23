/*The MainWindow class of the ImageEvolverQt program
 * Copyright (C) 2009-2010  Aaron Fan
 * Version 3.2
 *
 *This program is free software: you can redistribute it and/or modify
 *it under the terms of the GNU General Public License as published by
 *the Free Software Foundation, either version 3 of the License, or
 *(at your option) any later version.
 *
 *This program is distributed in the hope that it will be useful,
 *but WITHOUT ANY WARRANTY; without even the implied warranty of
 *MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *GNU General Public License for more details.
 *
 *You should have received a copy of the GNU General Public License
 *along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent), ui(new Ui::MainWindow),
	fileopener(this, Qt::Dialog),
	updateTimer(this),
	saveIncremental(false), updateImage(true),
	lastUpdate(0),
	version("v3.2")
{
    ui->setupUi(this);

    this->setWindowTitle(QString("Image Evolver " + version));

    fileopener.setDirectory(QDir::current());
    QList<QUrl> sidebarURLs = fileopener.sidebarUrls();
    sidebarURLs << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::PicturesLocation));
    fileopener.setSidebarUrls(sidebarURLs);

    connect(&updateTimer, SIGNAL(timeout()), this, SLOT(update()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadImage()
{
    bool wasRunning = imageMutator.mutateIsOn;
	imageMutator.stopMutation();

    updateTimer.stop();

    fileopener.setAcceptMode(QFileDialog::AcceptOpen);
    fileopener.setFileMode(QFileDialog::ExistingFile);
    fileopener.setFilter(tr("Image Files(*.png *.jpg *.jpeg *.bmp *.gif *.pbm *.pgm *.ppm *.tiff *.xbm *.xpm)"));

    QString path;
    if (fileopener.exec())
    {
        path = fileopener.selectedFiles().at(0);
        autosaveFilename = path;
        autosaveFilename = autosaveFilename.remove(autosaveFilename.length()-3, 3) + QString("autosave.svg");
        incrementalFilename = path;
        incrementalFilename = incrementalFilename.remove(incrementalFilename.length()-3, 3);

        if (!imageMutator.loadImage(path))
        {
            QMessageBox::critical(this, tr("File Read Error"), tr("Errored while loading image"));
            return;
        }

        updateOriginalImage();
        updateCurrentImage();
        updateStatus();
		lastUpdate = 0;

		ui->startButton->setText(QString("S&tart"));
        ui->startButton->setEnabled(true);
        ui->loadSVGButton->setEnabled(true);
        ui->backgroundColorButton->setEnabled(true);
    }
    else if (wasRunning)
    {
        imageMutator.startMutation();
        updateTimer.start(Constants::C_UPDATE_SPEED);
    }
}

void MainWindow::loadSVG()
{
    bool wasRunning = imageMutator.mutateIsOn;
	imageMutator.stopMutation();

    updateTimer.stop();

    fileopener.setAcceptMode(QFileDialog::AcceptOpen);
    fileopener.setFileMode(QFileDialog::ExistingFile);
    fileopener.setFilter(tr("SVG Files(*.svg)"));

    QString path;
    if (fileopener.exec())
    {
        path = fileopener.selectedFiles().at(0);
        if (!imageMutator.loadSVG(path))
        {
            QMessageBox::critical(this, tr("File Read Error"), tr("Errored while loading image"));
            return;
        }
        updateCurrentImage();
        updateStatus();
    }
    else if (wasRunning)
    {
        imageMutator.startMutation();
        updateTimer.start(Constants::C_UPDATE_SPEED);
    }
}

void MainWindow::toggleMutation()
{
    if (!imageMutator.mutateIsOn)
    {
        updateTimer.start(Constants::C_UPDATE_SPEED);
        imageMutator.startMutation();
		ui->startButton->setText(QString("S&top"));
        ui->setShapesButton->setEnabled(false);
    }
    else
    {
        updateTimer.stop();
        imageMutator.stopMutation();
		ui->startButton->setText(QString("S&tart"));
        ui->setShapesButton->setEnabled(true);
    }
}

void MainWindow::saveIncrementalChanged(bool input)
{
    saveIncremental = input;
}

void MainWindow::saveImage()
{
    bool wasRunning = imageMutator.mutateIsOn;
	imageMutator.stopMutation();

    updateTimer.stop();

    fileopener.setAcceptMode(QFileDialog::AcceptSave);
    fileopener.setFileMode(QFileDialog::AnyFile);
    fileopener.setFilter(tr("PNG Files(*.png);;SVG Files(*.svg)"));

    if (fileopener.exec())
    {
        QString path = fileopener.selectedFiles().at(0);
        QString filter = fileopener.selectedNameFilter();
        if (filter == "PNG Files(*.png)")
        {
            if (!imageMutator.saveBestImage(path + (path.endsWith(QString(".png"), Qt::CaseInsensitive) ? "" : ".png")))
            {
                QMessageBox::critical(this, tr("File Write Error"), tr("Errored while saving image"));
            }
        }
        else
        {
            if (!imageMutator.saveSVG(path + (path.endsWith(QString(".svg"), Qt::CaseInsensitive) ? "" : ".svg")))
            {
                QMessageBox::critical(this, tr("File Write Error"), tr("Errored while saving image"));
            }
        }
    }
    if (wasRunning)
    {
		imageMutator.startMutation();
        updateTimer.start(Constants::C_UPDATE_SPEED);
    }
}

void MainWindow::autosave()
{
    imageMutator.saveSVG(autosaveFilename);
    if (saveIncremental)
    {
        QDateTime theTime = QDateTime::currentDateTime();
        imageMutator.saveBestImage(incrementalFilename + theTime.toString(QString("ddMMyyhhmmsszzz")) + QString(".png"));
    }
}

void MainWindow::showAbout()
{
    AboutDialog ad(this);
    ad.exec();
}

void MainWindow::changeBackground()
{
    QColor bgColor = QColorDialog::getColor(imageMutator.getBackground(), this);
    if (bgColor.isValid())
    {
        imageMutator.setBackground(bgColor);
        update();
    }
}

void MainWindow::setShapes()
{
    ShapeChooser sc(this, &imageMutator);
    sc.exec();
}

void MainWindow::updateImageToggled(bool in)
{
    updateImage = in;
}

void MainWindow::update()
{
    updateStatus();
    if (updateImage)
        updateCurrentImage();
	unsigned int goodMuts = imageMutator.getGoodMutationCount();
	if (goodMuts - lastUpdate > (unsigned int)Constants::C_AUTOSAVE_SPEED)
	{
		lastUpdate = goodMuts;
		autosave();
	}
}

void MainWindow::updateStatus()
{
    QString report =
        "Mutation Count: " + QString::number(imageMutator.getGoodMutationCount()) + "/" + QString::number(imageMutator.getTestedMutationCount()) +
		"   Score: " + QString::number(imageMutator.getCurrentScore()) + "%" +
        "   Number of Polygons: " + QString::number(imageMutator.getNumberOfPolygons()) +
        "   Number of Threads: " + QString::number(imageMutator.countThreads());

    emit updateStatusBar(report);
}

void MainWindow::updateCurrentImage()
{
    QImage temp;
    imageMutator.currentImageLock.lock();
    if (imageMutator.getCurrentImage().width() > imageMutator.getCurrentImage().height())
        temp = imageMutator.getCurrentImage().scaledToWidth(ui->currentImageLabel->width());
    else
        temp = imageMutator.getCurrentImage().scaledToHeight(ui->currentImageLabel->height());
    imageMutator.currentImageLock.unlock();
    ui->currentImageLabel->setPixmap(QPixmap::fromImage(temp));
}

void MainWindow::updateOriginalImage()
{
    QImage temp;
    if (imageMutator.getOriginalImage().width() > imageMutator.getOriginalImage().height())
        temp = imageMutator.getOriginalImage().scaledToWidth(ui->originalImageLabel->width());
    else
        temp = imageMutator.getOriginalImage().scaledToHeight(ui->originalImageLabel->height());
    ui->originalImageLabel->setPixmap(QPixmap::fromImage(temp));
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    if (ui->startButton->isEnabled() == true)
    {
        updateOriginalImage();
        updateCurrentImage();
		event->accept();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    imageMutator.stopMutation();
    event->accept();
}
