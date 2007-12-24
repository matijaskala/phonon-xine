/*  This file is part of the KDE project
    Copyright (C) 2006-2007 Matthias Kretz <kretz@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License version 2 as published by the Free Software Foundation.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.

*/

#include "loadfakebackend.h"
#include <QtCore/QObject>
#include <qtest_kde.h>
#include "../objectdescription.h"

class VideoCaptureDeviceTest : public QObject
{
    Q_OBJECT
    private Q_SLOTS:
        void initTestCase();
        void sensibleValues();
        void cleanupTestCase();
};

using namespace Phonon;

void VideoCaptureDeviceTest::initTestCase()
{
    QCoreApplication::setApplicationName("videocapturedevicetest");
    Phonon::loadFakeBackend();
}

void VideoCaptureDeviceTest::sensibleValues()
{
    VideoCaptureDevice a;
    QCOMPARE(a.isValid(), false);
    VideoCaptureDevice b(a);
    QCOMPARE(b.isValid(), false);
    b = a;
    QCOMPARE(b.isValid(), false);

    if (Factory::backendName() == QLatin1String("Fake")) {
        VideoCaptureDevice c = VideoCaptureDevice::fromIndex(30000);
        QCOMPARE(c.isValid(), true);
        QCOMPARE(c.index(), 30000);
        QCOMPARE(c.name(), QString("USB Webcam"));
        QCOMPARE(c.description(), QString("first description"));
        b = VideoCaptureDevice::fromIndex(30001);
        QCOMPARE(b.isValid(), true);
        QCOMPARE(b.index(), 30001);
        QCOMPARE(b.name(), QString("DV"));
        QCOMPARE(b.description(), QString("second description"));
        QCOMPARE(a.isValid(), false);
        a = c;
        QCOMPARE(a, c);
        QCOMPARE(a.isValid(), true);
        QCOMPARE(a.index(), 30000);
        QCOMPARE(a.name(), QString("USB Webcam"));
        QCOMPARE(a.description(), QString("first description"));
    //} else {
    }
}

void VideoCaptureDeviceTest::cleanupTestCase()
{
}

QTEST_MAIN(VideoCaptureDeviceTest)
#include "videocapturedevicetest.moc"
// vim: sw=4 ts=4