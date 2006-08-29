/*  This file is part of the KDE project
    Copyright (C) 2004-2006 Matthias Kretz <kretz@kde.org>

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

#ifndef Phonon_XINE_UI_BACKEND_H
#define Phonon_XINE_UI_BACKEND_H

#include <QObject>
#include <kdelibs_export.h>

namespace Phonon
{
namespace Xine
{
	class KDE_EXPORT UiBackend : public QObject
	{
		Q_OBJECT
		public:
			UiBackend( QObject* parent, const QStringList& args );
			~UiBackend();

		public slots:
			QObject* createVideoWidget( QWidget* parent );
	};
}} // namespace Phonon::Xine

// vim: sw=4 ts=4 noet tw=80
#endif // Phonon_XINE_UI_BACKEND_H
