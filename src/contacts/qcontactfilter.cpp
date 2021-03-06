/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtContacts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qcontactfilter.h"
#include "qcontactfilter_p.h"

#ifndef QT_NO_DATASTREAM
#include <QtCore/qdatastream.h>
#endif
#ifndef QT_NO_DEBUG_STREAM
#include <QtCore/qdebug.h>
#endif

#include "qcontactfilters.h"
#include "qcontactmanager.h"

/*!
  \class QContactFilter
  \brief The QContactFilter class is used to select contacts made available
  through a QContactManager.

  \inmodule QtContacts

  \ingroup contacts-main

  This class is used as a parameter to various functions offered by QContactManager, to allow
  selection of contacts which have certain details or properties.
 */

/*!
  \enum QContactFilter::FilterType
  Describes the type of the filter
  \value InvalidFilter An invalid filter which matches nothing
  \value ContactDetailFilter A filter which matches contacts containing one or more details of a particular definition with a particular value
  \value ContactDetailRangeFilter A filter which matches contacts containing one or more details of a particular definition whose values are within a particular range
  \value ChangeLogFilter A filter which matches contacts whose timestamps have been updated since some particular date and time
  \value ActionFilter A filter which matches contacts for which a particular action is available, or which contain a detail with a particular value for which a particular action is available
  \value RelationshipFilter A filter which matches contacts which participate in a particular type of relationship, or relationship with a specified contact
  \value IntersectionFilter A filter which matches all contacts that are matched by all filters it includes
  \value UnionFilter A filter which matches any contact that is matched by any of the filters it includes
  \value IdFilter A filter which matches any contact whose id is contained in a particular list of contact ids
  \value DefaultFilter A filter which matches everything
 */

/*!
  \enum QContactFilter::MatchFlag
  Describes the semantics of matching followed by the filter
  \value MatchExactly Performs QVariant-based matching , combination of MatchExactly with other flags is not supported
  \value MatchContains The search term is contained in the item
  \value MatchStartsWith The search term matches the start of the item
  \value MatchEndsWith The search term matches the end of the item
  \value MatchFixedString Performs string-based matching. String-based comparisons are case-insensitive unless the \c MatchCaseSensitive flag is also specified
  \value MatchCaseSensitive The search is case sensitive
  \value MatchPhoneNumber The search term is considered to be in the form of a phone number, and special processing (removing dialing prefixes, non significant
         characters like '-'. ')' etc). may be performed when matching the item.
  \value MatchKeypadCollation The search term is in the form of text entered by a numeric phone keypad (such as ITU-T E.161 compliant keypads).  Each digit in the
         search term can represent a number of alphanumeric symbols.  For example, the search string "43556" would match items "HELLO", "GEKKO", "HELL6" and "43556" among others.
         Accented characters and other punctuation characters may additionally be matched by the QContactManager in a way consistent with the platform.
 */

/*!
  \fn QContactFilter::operator!=(const QContactFilter& other) const
  Returns true if this filter is not identical to the \a other filter.
  \sa operator==()
 */

#if !defined(Q_CC_MWERKS)
template<> QTCONTACTS_PREPEND_NAMESPACE(QContactFilterPrivate) *QSharedDataPointer<QTCONTACTS_PREPEND_NAMESPACE(QContactFilterPrivate)>::clone()
{
    return d->clone();
}
#endif

QT_BEGIN_NAMESPACE_CONTACTS

/*! Constructs an empty filter */
QContactFilter::QContactFilter()
    : d_ptr(0)
{
}

/*! Constructs a new copy of \a other */
QContactFilter::QContactFilter(const QContactFilter& other)
    : d_ptr(other.d_ptr)
{
}

/*! Assigns this filter to be \a other
*/
QContactFilter& QContactFilter::operator=(const QContactFilter& other)
{
    if (this != &other) {
        d_ptr = other.d_ptr;
    }
    return *this;
}

/*! Cleans up the memory used by this filter */
QContactFilter::~QContactFilter()
{
}

/*! Returns the type of the filter */
QContactFilter::FilterType QContactFilter::type() const
{
    if (!d_ptr)
        return QContactFilter::DefaultFilter;
    return d_ptr->type();
}

/*! Returns true if the filter has the same type and criteria as \a other
*/
bool QContactFilter::operator==(const QContactFilter& other) const
{
    /* A default filter is only equal to other default filters */
    if (!d_ptr)
        return !other.d_ptr;

    /* Different types can't be equal */
    if (other.type() != type())
        return false;

    /* Otherwise, use the virtual op == */
    return d_ptr->compare(other.d_ptr);
}

#ifndef QT_NO_DATASTREAM
/*!
 * Writes \a filter to the stream \a out.
 */
QDataStream& operator<<(QDataStream& out, const QContactFilter& filter)
{
    quint8 formatVersion = 1; // Version of QDataStream format for QContactDetailFilter
    out << formatVersion << static_cast<quint32>(filter.type());
    if (filter.d_ptr)
        filter.d_ptr->outputToStream(out, formatVersion);
    return out;
}

/*!
 * Reads a contact filter from stream \a in into \a filter.
 */
QDataStream& operator>>(QDataStream& in, QContactFilter& filter)
{
    filter = QContactFilter();
    quint8 formatVersion;
    in >> formatVersion;
    if (formatVersion == 1) {
        quint32 type;
        in >> type;
        switch (type) {
            case QContactFilter::InvalidFilter:
                filter = QContactInvalidFilter();
                break;
            case QContactFilter::ContactDetailFilter:
                filter = QContactDetailFilter();
                break;
            case QContactFilter::ContactDetailRangeFilter:
                filter = QContactDetailRangeFilter();
                break;
            case QContactFilter::ChangeLogFilter:
                filter = QContactChangeLogFilter();
                break;
            case QContactFilter::ActionFilter:
                filter = QContactActionFilter();
                break;
            case QContactFilter::RelationshipFilter:
                filter = QContactRelationshipFilter();
                break;
            case QContactFilter::IntersectionFilter:
                filter = QContactIntersectionFilter();
                break;
            case QContactFilter::UnionFilter:
                filter = QContactUnionFilter();
                break;
            case QContactFilter::IdFilter:
                filter = QContactIdFilter();
                break;
            case QContactFilter::DefaultFilter:
                filter = QContactFilter();
                break;
        }
        if (filter.d_ptr)
            filter.d_ptr->inputFromStream(in, formatVersion);
    } else {
        in.setStatus(QDataStream::ReadCorruptData);
    }
    return in;
}

#endif

#ifndef QT_NO_DEBUG_STREAM
/*!
  Outputs \a filter to the debug stream \a dbg
 */
QDebug operator<<(QDebug dbg, const QContactFilter& filter)
{
    dbg.nospace() << "QContactFilter(";
    if (filter.d_ptr)
        filter.d_ptr->debugStreamOut(dbg);
    else
        dbg.nospace() << "(null)";
    dbg.nospace() << ")";
    return dbg.maybeSpace();
}
#endif

/*!
  \internal
  Constructs a new filter from the given data pointer \a d
 */
QContactFilter::QContactFilter(QContactFilterPrivate *d)
    : d_ptr(d)
{

}

/*!
 \relates QContactFilter
 Returns a filter which is the intersection of the \a left and \a right filters
 \sa QContactIntersectionFilter
 */
const QContactFilter operator&(const QContactFilter& left, const QContactFilter& right)
{
    // XXX TODO: empty intersection/union operations are not well defined yet.
    //if (left.type() == QContactFilter::Intersection) {
    //    QContactIntersectionFilter bf(left);
    //    /* we can just add the right to this one */
    //    bf.append(right);
    //    return bf;
    //}

    //if (right.type() == QContactFilter::Intersection) {
    //    QContactIntersectionFilter bf(right);
    //    /* we can prepend the left to this one */
    //    bf.prepend(left);
    //    return bf;
    //}

    /* usual fallback case */
    QContactIntersectionFilter nif;
    nif << left << right;
    return nif;
}

/*!
 \relates QContactFilter
 Returns a filter which is the union of the \a left and \a right filters
 \sa QContactUnionFilter
 */
const QContactFilter operator|(const QContactFilter& left, const QContactFilter& right)
{
    if (left.type() == QContactFilter::UnionFilter) {
        QContactUnionFilter bf(left);
        /* we can just add the right to this one */
        bf.append(right);
        return bf;
    }

    if (right.type() == QContactFilter::UnionFilter) {
        QContactUnionFilter bf(right);
        /* we can prepend the left to this one */
        bf.prepend(left);
        return bf;
    }

    /* usual fallback case */
    QContactUnionFilter nif;
    nif << left << right;
    return nif;
}

QT_END_NAMESPACE_CONTACTS
