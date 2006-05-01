/*  This file is part of the KDE project
    Copyright (C) 2006 Tim Beaulen <tbscope@gmail.com>

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

#include "abstractmediaproducer.h"
#include <QTimer>
#include "videopath.h"
#include "audiopath.h"
#include <kdebug.h>
#include <QVector>
#include <cmath>
#include <QFile>
#include <QByteArray>

namespace Phonon
{
namespace Xine
{
static const int SAMPLE_RATE = 44100;
static const float SAMPLE_RATE_FLOAT = 44100.0f;

AbstractMediaProducer::AbstractMediaProducer( QObject* parent )
	: QObject( parent )
	, m_state( Phonon::LoadingState )
	, m_tickTimer( new QTimer( this ) )
	, m_bufferSize( 512 )
	, m_lastSamplesMissing( 0 )
	, m_position( 0.0f )
	, m_frequency( 440.0f )
{
	//kDebug() << k_funcinfo << endl;
	connect( m_tickTimer, SIGNAL( timeout() ), SLOT( emitTick() ) );
}

AbstractMediaProducer::~AbstractMediaProducer()
{
	//kDebug() << k_funcinfo << endl;
}

void AbstractMediaProducer::setBufferSize( int size )
{
	m_bufferSize = size;
}

bool AbstractMediaProducer::addVideoPath( Ifaces::VideoPath* videoPath )
{
	//kDebug() << k_funcinfo << endl;
	Q_ASSERT( videoPath );
	VideoPath* vp = qobject_cast<VideoPath*>( videoPath->qobject() );
	Q_ASSERT( vp );
	return true;
}

bool AbstractMediaProducer::addAudioPath( Ifaces::AudioPath* audioPath )
{
	//kDebug() << k_funcinfo << endl;
	Q_ASSERT( audioPath );
	AudioPath* ap = qobject_cast<AudioPath*>( audioPath->qobject() );
	Q_ASSERT( ap );
	Q_ASSERT( !m_audioPathList.contains( ap ) );
	m_audioPathList.append( ap );
	return true;
}

void AbstractMediaProducer::removeVideoPath( Ifaces::VideoPath* videoPath )
{
	Q_ASSERT( videoPath );
	VideoPath* vp = qobject_cast<VideoPath*>( videoPath->qobject() );
	Q_ASSERT( vp );
}

void AbstractMediaProducer::removeAudioPath( Ifaces::AudioPath* audioPath )
{
	Q_ASSERT( audioPath );
	AudioPath* ap = qobject_cast<AudioPath*>( audioPath->qobject() );
	Q_ASSERT( ap );
	Q_ASSERT( m_audioPathList.contains( ap ) );
	m_audioPathList.removeAll( ap );
}

State AbstractMediaProducer::state() const
{
	//kDebug() << k_funcinfo << endl;
	return m_state;
}

bool AbstractMediaProducer::hasVideo() const
{
	//kDebug() << k_funcinfo << endl;
	return false;
}

bool AbstractMediaProducer::seekable() const
{
	//kDebug() << k_funcinfo << endl;
	return true;
}

long AbstractMediaProducer::currentTime() const
{
	//kDebug() << k_funcinfo << endl;
	switch( state() )
	{
		case Phonon::PausedState:
		case Phonon::BufferingState:
			return m_startTime.msecsTo( m_pauseTime );
		case Phonon::PlayingState:
			return m_startTime.elapsed();
		case Phonon::StoppedState:
		case Phonon::LoadingState:
			return 0;
		case Phonon::ErrorState:
			break;
	}
	return -1;
}

long AbstractMediaProducer::tickInterval() const
{
	//kDebug() << k_funcinfo << endl;
	return m_tickInterval;
}

long AbstractMediaProducer::setTickInterval( long newTickInterval )
{
	//kDebug() << k_funcinfo << endl;
	m_tickInterval = newTickInterval;
	if( m_tickInterval <= 0 )
		m_tickTimer->setInterval( 50 );
	else
		m_tickTimer->setInterval( newTickInterval );
	return m_tickInterval;
}

void AbstractMediaProducer::play()
{
	//kDebug() << k_funcinfo << endl;
	m_tickTimer->start();
	setState( Phonon::PlayingState );
}

void AbstractMediaProducer::pause()
{
	//kDebug() << k_funcinfo << endl;
	m_tickTimer->stop();
	setState( Phonon::PausedState );
}

void AbstractMediaProducer::stop()
{
	//kDebug() << k_funcinfo << endl;
	m_tickTimer->stop();
	setState( Phonon::StoppedState );
	m_position = 0.0f;
	m_frequency = 440.0f;
}

void AbstractMediaProducer::seek( long time )
{
	//kDebug() << k_funcinfo << endl;
	if( seekable() )
	{
		switch( state() )
		{
			case Phonon::PausedState:
			case Phonon::BufferingState:
				m_startTime = m_pauseTime;
				break;
			case Phonon::PlayingState:
				m_startTime = QTime::currentTime();
				break;
			case Phonon::StoppedState:
			case Phonon::ErrorState:
			case Phonon::LoadingState:
				return; // cannot seek
		}
		m_startTime = m_startTime.addMSecs( -time );
	}
}

void AbstractMediaProducer::setState( State newstate )
{
	if( newstate == m_state )
		return;
	State oldstate = m_state;
	m_state = newstate;
	switch( newstate )
	{
		case Phonon::PausedState:
		case Phonon::BufferingState:
			m_pauseTime.start();
			break;
		case Phonon::PlayingState:
			if( oldstate == Phonon::PausedState || oldstate == Phonon::BufferingState )
				m_startTime = m_startTime.addMSecs( m_pauseTime.elapsed() );
			else
				m_startTime.start();
			break;
		case Phonon::StoppedState:
		case Phonon::ErrorState:
		case Phonon::LoadingState:
			break;
	}
	//kDebug() << "emit stateChanged( " << newstate << ", " << oldstate << " )" << endl;
	emit stateChanged( newstate, oldstate );
}

void AbstractMediaProducer::emitTick()
{
	//kDebug( 604 ) << "emit tick( " << currentTime() << " )" << endl;
	int tickInterval = 50;
	if( m_tickInterval > 0 )
	{
		emit tick( currentTime() );
		tickInterval = m_tickInterval;
	}
	QVector<float> buffer( m_bufferSize );

	const int availableSamples = tickInterval * SAMPLE_RATE / 1000 + m_lastSamplesMissing;
	const int bufferCount = availableSamples / m_bufferSize;
	m_lastSamplesMissing = availableSamples - bufferCount * m_bufferSize;
	for( int i = 0; i < bufferCount; ++i )
	{
		fillBuffer( &buffer );
		foreach( AudioPath* ap, m_audioPathList )
			ap->processBuffer( buffer );
	}
}

static const float TWOPI = 6.28318530718f;
static const float maxFrequency = 1760.0f;
static const float minFrequency = 440.0f;
static const float frequencyToDelta = TWOPI / SAMPLE_RATE_FLOAT;

void AbstractMediaProducer::fillBuffer( QVector<float>* buffer )
{
	//static QFile createdump( "createdump" );
	//if( !createdump.isOpen() )
		//createdump.open( QIODevice::WriteOnly );

	m_frequency *= 1.059463094359f;
	if( m_frequency > maxFrequency )
		m_frequency = minFrequency;
	float delta = frequencyToDelta * m_frequency;

	float* data = buffer->data();
	const float * const end = data + m_bufferSize;

	while( data != end )
	{
		const float sample = sinf( m_position );
		//createdump.write( QByteArray::number( sample ) + "\n" );
		*( data++ ) = sample;
		m_position += delta;
		if( m_position > TWOPI )
			m_position -= TWOPI;
	}
}

}}
#include "abstractmediaproducer.moc"
// vim: sw=4 ts=4 noet
