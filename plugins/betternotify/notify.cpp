/*
    This file is part of Choqok, the KDE micro-blogging client

    Copyright (C) 2011-2012 Mehrdad Momeny <mehrdad.momeny@gmail.com>

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

#include "notify.h"

#include <QDesktopWidget>

#include <KPluginFactory>

#include "account.h"
#include "application.h"
#include "choqokuiglobal.h"
#include "mediamanager.h"
#include "postwidget.h"

#include "notification.h"
#include "notifysettings.h"

K_PLUGIN_FACTORY_WITH_JSON(NotifyFactory, "choqok_notify.json",
                           registerPlugin < Notify > ();)

Notify::Notify(QObject *parent, const QList< QVariant > &)
    : Choqok::Plugin(QLatin1String("choqok_betternotify"), parent), notification(0)
{
    NotifySettings set;
    accountsList = set.accounts();
    timer.setInterval(set.notifyInterval() * 1000);
    connect(Choqok::UI::Global::SessionManager::self(),
            SIGNAL(newPostWidgetAdded(Choqok::UI::PostWidget*,Choqok::Account*,QString)),
            this,
            SLOT(slotNewPostWidgetAdded(Choqok::UI::PostWidget*,Choqok::Account*,QString)));
    connect(&timer, SIGNAL(timeout()), this, SLOT(notifyNextPost()));

    notifyPosition = set.position();
}

Notify::~Notify()
{
}

void Notify::slotNewPostWidgetAdded(Choqok::UI::PostWidget *pw, Choqok::Account *acc, QString tm)
{
//     qDebug()<<Choqok::Application::isStartingUp()<< Choqok::Application::isShuttingDown();
    if (Choqok::Application::isStartingUp() || Choqok::Application::isShuttingDown()) {
        //qDebug()<<"Choqok is starting up or going down!";
        return;
    }
    if (pw && !pw->isRead() && accountsList[acc->alias()].contains(tm)) {
        //qDebug()<<"POST ADDED TO NOTIFY IT: "<<pw->currentPost()->content;
        postQueueToNotify.enqueue(pw);
        if (!timer.isActive()) {
            notifyNextPost();
            timer.start();
        }
    }
}

void Notify::notifyNextPost()
{
    if (postQueueToNotify.isEmpty()) {
        timer.stop();
        if (notification) {
            hideLastNotificationAndShowThis();
        }
    } else {
        notify(postQueueToNotify.dequeue());
    }
}

void Notify::notify(QPointer< Choqok::UI::PostWidget > post)
{
    if (post) {
        Notification *notif = new Notification(post);
        connect(notif, SIGNAL(ignored()), this, SLOT(stopNotifications()));
        connect(notif, SIGNAL(postReaded()), SLOT(slotPostReaded()));
        connect(notif, SIGNAL(mouseEntered()), &timer, SLOT(stop()));
        connect(notif, SIGNAL(mouseLeaved()), &timer, SLOT(start()));
        hideLastNotificationAndShowThis(notif);
    } else  {
        hideLastNotificationAndShowThis();
    }
}

void Notify::slotPostReaded()
{
    notifyNextPost();
    timer.stop();
    timer.start();
}

void Notify::stopNotifications()
{
    postQueueToNotify.clear();
    timer.stop();
    hideLastNotificationAndShowThis();
}

void Notify::hideLastNotificationAndShowThis(Notification *nextNotificationToShow)
{
    //TODO: Add Animation
    notification->deleteLater();
    notification = 0;
    if (nextNotificationToShow) {
        notification = nextNotificationToShow;
        notification->move(notifyPosition);
        notification->show();
    }
}

#include "notify.moc"
