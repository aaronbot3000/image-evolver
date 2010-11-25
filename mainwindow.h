/*The MainWindow class of the ImageEvolverQt program
 * Copyright (C) 2009-2010  Aaron Fan
 * Version 3.3
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QCloseEvent>
#include <QColorDialog>
#include <QDateTime>
#include <QDesktopServices>
#include <QDir>
#include <QFileDialog>
#include <QList>
#include <QMessageBox>
#include <QtGui/QMainWindow>
#include <QTimer>
#include <QUrl>

#include "aboutdialog.h"
#include "imagemutationcore.h"
#include "shapechooser.h"
#include "ui_mainwindow.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = 0);
        ~MainWindow();
        
    public slots:
        void autosave();
        void changeBackground();
        void closeEvent(QCloseEvent *event);
        void loadImage();
        void loadSVG();
        void saveImage();
        void saveIncrementalChanged(bool input);
        void showAbout();
        void setShapes();
        void toggleMutation();
        void update();
        void updateImageToggled(bool in);

    signals:
        void updateStatusBar(QString report);

    private:
        void updateStatus();
        void updateCurrentImage();
        void updateOriginalImage();
		Ui::MainWindow *ui;
        QFileDialog fileopener;
        QString autosaveFilename;
        QString incrementalFilename;
        QTimer updateTimer;
        ImageMutationCore imageMutator;
        bool saveIncremental;
        bool updateImage;

		unsigned int lastUpdate;

        const QString version;

	protected:
            virtual void resizeEvent(QResizeEvent *event);
};

#endif // MAINWINDOW_H
