/*  This file is part of the KDE project
    Copyright (C) 2006 Matthias Kretz <kretz@kde.org>

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

#include "backend.h"
#include "videowidget.h"
#include <kgenericfactory.h>
#include <kdebug.h>

typedef KGenericFactory<Phonon::Xine::UiBackend> XineUiUiBackendFactory;
K_EXPORT_COMPONENT_FACTORY( phonon_xineui, XineUiUiBackendFactory( "xineuibackend" ) )

namespace Phonon
{
namespace Xine
{

UiBackend::UiBackend( QObject* parent, const QStringList& )
	: QObject( parent )
{
	kDebug( 610 ) << k_funcinfo << endl;
}

UiBackend::~UiBackend()
{
	kDebug( 610 ) << k_funcinfo << endl;
}

QObject* UiBackend::createVideoWidget( QWidget* parent )
{
	kDebug( 610 ) << k_funcinfo << endl;
    Xine::VideoWidget *vw = new Xine::VideoWidget(parent);
    if (vw->isValid()) {
        return vw;
    }
    delete vw;
    return 0;
}

}}

#include "backend.moc"
// vim: sw=4 ts=4
