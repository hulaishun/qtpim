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

#ifndef QORGANIZERJSONDBENGINE_H
#define QORGANIZERJSONDBENGINE_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtOrganizer/qorganizermanagerengine.h>

QTORGANIZER_BEGIN_NAMESPACE

/*
 Organizer jsondb backend storage location related documentation
 (will be moved later to proper location)

 Storage locations are locations where the organizer data can be stored. Currently there
 are two locations; UserDataStorage and SystemStorage. These can be seen on the
 QOrganizerAbstractRequest API's QOrganizerAbstractRequest::StorageLocation.

 Organizer jsondb backend engine requires storage locations (partitions in qtjsondb-module
 terminology) to be present. If StorageLocationsNotExistingError is received from requests,
 it means storage locations are missing or cannot be accessed due to missing security
 access rights and Organizer jsondb backend cannot work properly.

 If storage locations are missing, one can easily create those via jsondb-client utility.
 When jsondb is running, launch also jsondb-client and write:
 ...
 create {"_type": "Partition", "name": "com.nokia.mt.System"}
 create {"_type": "Partition", "name": "com.nokia.mt.User"}
 ...

 After this, the organizer should be able to use jsondb backend and does not return
 StorageLocationsNotExistingError anymore.

 If you use QOrganizerManagerEngine APIs, please note that the QOrganizerJsonDbEngineData's
 implementation directs all the operations to UserDataStorage storage location. If you want to
 target operations to other storage locations, you need to use the asynchronous requests
 instead.

 There are some restrictions with organizer data and storing it to storage
 locations. Once an item or collection is saved to one storage location, you cannot
 change it to another storage location. A collection and all the items it contains must also
 exist in the same storage location.

 By default items and collections are stored in UserDataStorage storage location and fetched
 from there.

 Storage location information is included in the engine item id syntax in following way:
 [QOrganizerAbstractRequest::StorageLocation]/[jsondb uuid]
*/

class QOrganizerJsonDbRequestThread;

class QOrganizerJsonDbEngine : public QOrganizerManagerEngine
{
    Q_OBJECT

public:
    QOrganizerJsonDbEngine(QOrganizerManager::Error *error);
    ~QOrganizerJsonDbEngine();

    /* URI reporting */
    QString managerName() const;
    QMap<QString, QString> managerParameters() const;
    int managerVersion() const;

    // items
    QList<QOrganizerItem> items(const QList<QOrganizerItemId> &itemIds, const QOrganizerItemFetchHint &fetchHint,
                                QMap<int, QOrganizerManager::Error> *errorMap, QOrganizerManager::Error *error);

    QList<QOrganizerItem> items(const QOrganizerItemFilter &filter, const QDateTime &startDateTime,
                                const QDateTime &endDateTime, int maxCount,
                                const QList<QOrganizerItemSortOrder> &sortOrders,
                                const QOrganizerItemFetchHint &fetchHint, QOrganizerManager::Error *error);

    QList<QOrganizerItemId> itemIds(const QOrganizerItemFilter &filter, const QDateTime &startDateTime,
                                    const QDateTime &endDateTime, const QList<QOrganizerItemSortOrder> &sortOrders,
                                    QOrganizerManager::Error *error);

    QList<QOrganizerItem> itemOccurrences(const QOrganizerItem &parentItem, const QDateTime &startDateTime,
                                          const QDateTime &endDateTime, int maxCount,
                                          const QOrganizerItemFetchHint &fetchHint, QOrganizerManager::Error *error);

    QList<QOrganizerItem> itemsForExport(const QDateTime &startDateTime, const QDateTime &endDateTime,
                                         const QOrganizerItemFilter &filter,
                                         const QList<QOrganizerItemSortOrder> &sortOrders,
                                         const QOrganizerItemFetchHint &fetchHint, QOrganizerManager::Error *error);

    bool saveItems(QList<QOrganizerItem> *items, const QList<QOrganizerItemDetail::DetailType> &detailMask,
                   QMap<int, QOrganizerManager::Error> *errorMap, QOrganizerManager::Error *error);

    bool removeItems(const QList<QOrganizerItemId> &itemIds, QMap<int, QOrganizerManager::Error> *errorMap,
                     QOrganizerManager::Error *error);

    bool removeItems(const QList<QOrganizerItem> *items, QMap<int, QOrganizerManager::Error>* errorMap,
                     QOrganizerManager::Error* error);

    // collections
    QOrganizerCollection defaultCollection(QOrganizerManager::Error* error);
    QOrganizerCollection collection(const QOrganizerCollectionId& collectionId, QOrganizerManager::Error* error);
    QList<QOrganizerCollection> collections(QOrganizerManager::Error* error);
    bool saveCollection(QOrganizerCollection* collection, QOrganizerManager::Error* error);
    bool removeCollection(const QOrganizerCollectionId& collectionId, QOrganizerManager::Error* error);

    /* Capabilities reporting */
    QList<QOrganizerItemFilter::FilterType> supportedFilters() const;
    QList<QOrganizerItemDetail::DetailType> supportedItemDetails(QOrganizerItemType::ItemType itemType) const;
    QList<QOrganizerItemType::ItemType> supportedItemTypes() const;

    /* Asynchronous Request Support */
    void requestDestroyed(QOrganizerAbstractRequest* req);
    bool startRequest(QOrganizerAbstractRequest* req);
    bool cancelRequest(QOrganizerAbstractRequest* req);
    bool waitForRequestFinished(QOrganizerAbstractRequest* req, int msecs);

signals:
    void requestReceived(QOrganizerAbstractRequest* req);

private:
    friend class QOrganizerJsonDbEngineFactory;
    QOrganizerJsonDbRequestThread *m_requestHandlerThread;
};

QTORGANIZER_END_NAMESPACE

#endif
