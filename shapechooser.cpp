/*The ShapeChooser class of the ImageEvolverQt program
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

#include "shapechooser.h"

ShapeChooser::ShapeChooser(QWidget *parent, ImageMutationCore *i): 
    QDialog(parent, Qt::Widget), ui(new Ui::ShapeChooser)
{
    ui->setupUi(this);
    im = i;
    ui->polygonCheckBox->setChecked(im->isShapeEnabled(0));
    ui->rectangleCheckBox->setChecked(im->isShapeEnabled(1));
    ui->ellipseCheckBox->setChecked(im->isShapeEnabled(2));
}

ShapeChooser::~ShapeChooser()
{
    delete ui;
}

void ShapeChooser::accept()
{
    if (ui->polygonCheckBox->isChecked() == false &&
        ui->rectangleCheckBox->isChecked() == false &&
        ui->ellipseCheckBox->isChecked() == false)
    {
        QMessageBox::critical(this, tr("Shape Choosing Human Error"), tr("Must have some shape checked"));
        return;
    }

    if (ui->polygonCheckBox->isChecked())
        im->enableShape(0);
    else
        im->disableShape(0);

    if (ui->rectangleCheckBox->isChecked())
        im->enableShape(1);
    else
        im->disableShape(1);

    if (ui->ellipseCheckBox->isChecked())
        im->enableShape(2);
    else
        im->disableShape(2);

    QDialog::accept();
}
