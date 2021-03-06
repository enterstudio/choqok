/*
    This file is part of Choqok, the KDE micro-blogging client

    Copyright (C) 2008-2012 Mehrdad Momeny <mehrdad.momeny@gmail.com>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License or (at your option) version 3 or any later version
    accepted by the membership of KDE e.V. (or its successor approved
    by the membership of KDE e.V.), which shall act as a proxy
    defined in Section 14 of version 3 of the license.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, see http://www.gnu.org/licenses/

*/

#include "editaccountdialog.h"

#include <QPushButton>
#include <QVBoxLayout>

#include <KConfigGroup>
#include <KLocalizedString>
#include <KMessageBox>

#include "account.h"
#include "accountmanager.h"
#include "accountsdebug.h"
#include "editaccountwidget.h"

EditAccountDialog::EditAccountDialog(ChoqokEditAccountWidget *editWidget, QWidget *parent, Qt::WindowFlags flags)
    : QDialog(parent, flags), widget(editWidget)
{
    if (!widget) {
        this->deleteLater();
        return;
    }

    setWindowTitle(i18n("Edit Account"));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    setLayout(mainLayout);
    mainLayout->addWidget(widget);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setDefault(true);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &EditAccountDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &EditAccountDialog::reject);
    mainLayout->addWidget(buttonBox);
}

EditAccountDialog::~EditAccountDialog()
{
}

void EditAccountDialog::accept()
{
    qCDebug(CHOQOK);
    if (widget->validateData()) {
        if (widget->apply()) {
            QDialog::accept();
        }
    } else {
        KMessageBox::sorry(this, i18n("Cannot validate your input information.\nPlease check the fields' data.\nMaybe a required field is empty?"));
    }
}

