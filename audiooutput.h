/*  This file is part of the KDE project
    Copyright (C) 2006 Tim Beaulen <tbscope@gmail.com>
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
#ifndef Phonon_XINE_AUDIOOUTPUT_H
#define Phonon_XINE_AUDIOOUTPUT_H

#include "abstractaudiooutput.h"
#include <QFile>

#include "xineengine.h"
#include <xine.h>
#include "xinestream.h"
#include "audioport.h"
#include <phonon/audiooutputinterface.h>

namespace Phonon
{
namespace Xine
{
	class AbstractMediaProducer;
    class AudioOutput : public AbstractAudioOutput, public AudioOutputInterface
	{
		Q_OBJECT
        Q_INTERFACES(Phonon::AudioOutputInterface)
		public:
            AudioOutput(QObject *parent);
			~AudioOutput();

            AudioPort audioPort() const;
			void updateVolume( AbstractMediaProducer* mp ) const;

			// Attributes Getters:
			float volume() const;
			int outputDevice() const;

			// Attributes Setters:
			void setVolume( float newVolume );
            bool setOutputDevice(int newDevice);

        protected:
            bool event(QEvent *);

		Q_SIGNALS:
			void volumeChanged( float newVolume );
            void audioDeviceFailed();

            void audioPortChanged(const AudioPort &);

		private:
			float m_volume;
			int m_device;
            AudioPort m_audioPort;
	};
}} //namespace Phonon::Xine

// vim: sw=4 ts=4 tw=80
#endif // Phonon_XINE_AUDIOOUTPUT_H
