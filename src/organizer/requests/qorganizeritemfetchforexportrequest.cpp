/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtOrganizer module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <qorganizeritemfetchforexportrequest.h>
#include <private/qorganizeritemrequests_p.h>

QTORGANIZER_BEGIN_NAMESPACE

/*!
    \class QOrganizerItemFetchForExportRequest
    \brief The QOrganizerItemFetchForExportRequest class allows a client to asynchronously fetch
           organizer items for export from a backend.
    \inmodule QtOrganizer
    \ingroup organizer-requests

    This request will only fetch parent items and persisted exceptions which match the specified
    criteria, and no generated occurrences will be fetched.
 */

/*!
    Constructs a new organizer item fetch for export request whose parent is the specified \a parent.
*/
QOrganizerItemFetchForExportRequest::QOrganizerItemFetchForExportRequest(QObject *parent)
    : QOrganizerAbstractRequest(new QOrganizerItemFetchForExportRequestPrivate, parent)
{
}

/*!
    Frees memory in use by this request.
*/
QOrganizerItemFetchForExportRequest::~QOrganizerItemFetchForExportRequest()
{
    QOrganizerAbstractRequestPrivate::notifyEngine(this);
}

/*!
    Sets the organizer item filter used to determine which organizer items will be retrieved to \a filter.
*/
void QOrganizerItemFetchForExportRequest::setFilter(const QOrganizerItemFilter &filter)
{
    Q_D(QOrganizerItemFetchForExportRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_filter = filter;
}

/*!
    Sets the sort order of the result to \a sorting.
*/
void QOrganizerItemFetchForExportRequest::setSorting(const QList<QOrganizerItemSortOrder> &sorting)
{
    Q_D(QOrganizerItemFetchForExportRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_sorting = sorting;
}

/*!
    Sets the fetch hint which may be used by the backend to optimize item retrieval to \a fetchHint.

    A client should not make changes to a item which has been retrieved using a fetch hint other than
    the default fetch hint.  Doing so will result in information loss when saving the item back to
    the manager (as the "new" restricted item will replace the previously saved item in the backend).
 */
void QOrganizerItemFetchForExportRequest::setFetchHint(const QOrganizerItemFetchHint &fetchHint)
{
    Q_D(QOrganizerItemFetchForExportRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_fetchHint = fetchHint;
}

/*!
    Sets the start period of the request to \a date.

    A default-constructed (invalid) start date time specifies an open start date time (matches anything
    which occurs up until the end date time).
*/
void QOrganizerItemFetchForExportRequest::setStartDate(const QDateTime &date)
{
    Q_D(QOrganizerItemFetchForExportRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_startDate = date;
}

/*!
    Sets the end period of the request to \a date.

    A default-constructed (invalid) end date time specifies an open end date time (matches anything
    which occurs after the start date time).
*/
void QOrganizerItemFetchForExportRequest::setEndDate(const QDateTime &date)
{
    Q_D(QOrganizerItemFetchForExportRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_endDate = date;
}

/*!
    Returns the filter that will be used to select organizer items to be returned.
*/
QOrganizerItemFilter QOrganizerItemFetchForExportRequest::filter() const
{
    Q_D(const QOrganizerItemFetchForExportRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_filter;
}

/*!
    Returns the sort ordering that will be used to sort the results of this request.
*/
QList<QOrganizerItemSortOrder> QOrganizerItemFetchForExportRequest::sorting() const
{
    Q_D(const QOrganizerItemFetchForExportRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_sorting;
}

/*!
    Returns the fetch hint which may be used by the backend to optimize item retrieval.

    A client should not make changes to a item which has been retrieved using a fetch hint other than
    the default fetch hint.  Doing so will result in information loss when saving the item back to
    the manager (as the "new" restricted item will replace the previously saved item in the backend).
 */
QOrganizerItemFetchHint QOrganizerItemFetchForExportRequest::fetchHint() const
{
    Q_D(const QOrganizerItemFetchForExportRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_fetchHint;
}

/*!
    Returns the date-time which is the lower bound for the range in which items will be returned.
 */
QDateTime QOrganizerItemFetchForExportRequest::startDate() const
{
    Q_D(const QOrganizerItemFetchForExportRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_startDate;
}

/*!
    Returns the date-time which is the upper bound for the range in which items will be returned.
 */
QDateTime QOrganizerItemFetchForExportRequest::endDate() const
{
    Q_D(const QOrganizerItemFetchForExportRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_endDate;
}

/*!
    Sets storage locations where the items are fetched from. \a storageLocations is a flag,
    so you can define multiple locations in it.

    \sa QOrganizerAbstractRequest::StorageLocation
    \sa QOrganizerItemFetchForExportRequest::storageLocations()
*/
void QOrganizerItemFetchForExportRequest::setStorageLocations(QOrganizerAbstractRequest::StorageLocations storageLocations)
{
    Q_D(QOrganizerItemFetchForExportRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_storageLocations = storageLocations;
}

/*!
    Storage locations where the items are fetched from.

    \sa QOrganizerAbstractRequest::StorageLocation
    \sa QOrganizerItemFetchForExportRequest::setStorageLocations()
*/
QOrganizerAbstractRequest::StorageLocations QOrganizerItemFetchForExportRequest::storageLocations() const
{
    Q_D(const QOrganizerItemFetchForExportRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_storageLocations;
}

/*!
    Returns the list of organizer items retrieved by this request.
*/
QList<QOrganizerItem> QOrganizerItemFetchForExportRequest::items() const
{
    Q_D(const QOrganizerItemFetchForExportRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_organizeritems;
}

#include "moc_qorganizeritemfetchforexportrequest.cpp"

QTORGANIZER_END_NAMESPACE
