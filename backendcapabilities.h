/*  This file is part of the KDE project
    Copyright (C) 2005-2006 Matthias Kretz <kretz@kde.org>

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

#ifndef Phonon_BACKENDCAPABILITIES_H
#define Phonon_BACKENDCAPABILITIES_H

#include <QObject>

#include <kdelibs_export.h>
#include <kstaticdeleter.h>
#include <kmimetype.h>

template<class T> class QList;

namespace Phonon
{
class AudioOutputDevice;
class AudioCaptureDevice;
class VideoCaptureDevice;

/**
 * Singleton class describing the capabilities of the Backend.
 *
 * \author Matthias Kretz <kretz@kde.org>
 */
class PHONONCORE_EXPORT BackendCapabilities : public QObject
{
	friend void ::KStaticDeleter<BackendCapabilities>::destructObject();

	Q_OBJECT
	public:
		/**
		 * Use this function to get an instance of Factory.
		 *
		 * \return a pointer to the BackendCapabilities instance. If no instance exists until now then
		 * one is created.
		 */
		static BackendCapabilities* self();

		/**
		 * \copydoc Phonon::Ifaces::Backend::supportsVideo
		 *
		 * If the backend does not support video the classes
		 * VideoPath, VideoEffect and all AbstractVideoOutput subclasses won't
		 * do anything.
		 */
		static bool supportsVideo();

		/**
		 * \copydoc Phonon::Ifaces::Backend::supportsOSD
		 */
		static bool supportsOSD();

		/**
		 * \copydoc Phonon::Ifaces::Backend::supportsSubtitles
		 */
		static bool supportsSubtitles();

		/**
		 * Returns a list of mime types that the Backend can decode.
		 */
		static KMimeType::List knownMimeTypes();

		/**
		 * Returns the audio output devices the backend supports.
		 *
		 * \return A list of AudioOutputDevice objects that give a name and
		 * description for every supported audio output device.
		 */
		static QList<AudioOutputDevice> availableAudioOutputDevices();

		/**
		 * Returns the audio capture devices the backend supports.
		 *
		 * \return A list of AudioCaptureDevice objects that give a name and
		 * description for every supported audio capture device.
		 */
		static QList<AudioCaptureDevice> availableAudioCaptureDevices();

		/**
		 * Returns the video capture devices the backend supports.
		 *
		 * \return A list of VideoCaptureDevice objects that give a name and
		 * description for every supported video capture device.
		 */
		static QList<VideoCaptureDevice> availableVideoCaptureDevices();

		static QStringList availableAudioEffects();
		static QStringList availableVideoEffects();

	Q_SIGNALS:
		/**
		 * This signal is emitted if the capabilites have changed. This can
		 * happen if the user has requested a backend change.
		 */
		void capabilitiesChanged();

	protected:
		BackendCapabilities();
		~BackendCapabilities();

	private Q_SLOTS:
		void slotBackendChanged();

	private:
		static BackendCapabilities* m_self;
		class Private;
		Private* d;
};
} // namespace Phonon

#endif // Phonon_BACKENDCAPABILITIES_H
// vim: sw=4 ts=4 tw=80 noet
