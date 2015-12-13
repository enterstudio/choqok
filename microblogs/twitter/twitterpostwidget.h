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

#ifndef TWITTERPOSTWIDGET_H
#define TWITTERPOSTWIDGET_H

#include "twitterapipostwidget.h"

class TwitterPostWidget : public TwitterApiPostWidget
{
    Q_OBJECT
public:
    TwitterPostWidget(Choqok::Account *account, Choqok::Post *post, QWidget *parent = 0);
    virtual void initUi();

protected Q_SLOTS:
    virtual void slotReplyToAll();
    void quotedAvatarFetched(const QString &remoteUrl, const QPixmap &pixmap);
    void quotedAvatarFetchError(const QString &remoteUrl, const QString &errMsg);

protected:
    virtual QString prepareStatus(const QString &text);
    virtual void checkAnchor(const QUrl &url);
    virtual bool isRemoveAvailable();
    bool setupQuotedAvatar();

    static const QRegExp mTwitterUserRegExp;
    static const QRegExp mTwitterTagRegExp;
    static const QString mQuotedTextBase;
    static const QUrl    mQuotedAvatarResourceUrl;
private:
    QString getBackgroundColor();
};

#endif // TWITTERPOSTWIDGET_H
