/*The AboutDialog class of the ImageEvolverQt program
 * Copyright (C) 2009-2010  Aaron Fan
 * Version 3.0
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

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QtGui/QDialog>

#include "ui_aboutdialog.h"

namespace Ui
{
    class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT

    public:
		AboutDialog(QWidget *parent = 0): 
			QDialog(parent, Qt::Widget), ui(new Ui::AboutDialog)
                        {ui->setupUi(this);}
		~AboutDialog(){delete ui;}
		Ui::AboutDialog *ui;
};

#endif // ABOUTDIALOG_H
