/*  This file is part of the KDE project
    Copyright (C) 2005 Matthias Kretz <kretz@kde.org>

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
#ifndef Phonon_MEDIAOBJECT_H
#define Phonon_MEDIAOBJECT_H

#include "phonon_export.h"
#include "phonondefs.h"
#include "abstractmediaproducer.h"

class KUrl;

namespace Phonon
{
    class MediaObjectPrivate;
    class KioFallbackImpl;

    /**
     * \short Interface for media playback of a given URL.
     *
     * This class is the most important class for most mediaplayback tasks. With
     * this class you open a URL, play, pause, stop, seek and get a lot of
     * information about the media data.
     *
     * A common usage example is the following:
     * \code
     * media = new MediaObject(this);
     * connect(media, SIGNAL(finished()), SLOT(slotFinished());
     * media->setUrl(KUrl("/home/username/music/filename.ogg"));
     * media->play();
     * \endcode
     *
     * \ingroup Playback
     * \author Matthias Kretz <kretz@kde.org>
     */
    class PHONONCORE_EXPORT MediaObject : public AbstractMediaProducer
    {
        friend class MediaQueue;
        friend class KioFallbackImpl;
        Q_OBJECT
        K_DECLARE_PRIVATE(MediaObject)
        PHONON_HEIR(MediaObject)
        Q_PROPERTY(KUrl url READ url WRITE setUrl)
        Q_PROPERTY(qint32 aboutToFinishTime READ aboutToFinishTime WRITE setAboutToFinishTime)
        public:
            enum Media {
                None = 0,
                CD = 1,
                DVD = 2,
                VCD = 3
            };
            /**
             * Get the URL the MediaObject should use as media data source.
             *
             * \return The currently used URL to the media data
             */
            KUrl url() const;

            Media media() const;

            /**
             * Get the total time (in milliseconds) of the file currently being played.
             *
             * \see length
             */
            qint64 totalTime() const;
            /**
             * Get the remaining time (in milliseconds) of the file currently being played.
             */
            qint64 remainingTime() const;

            /**
             * Returns the time in milliseconds the aboutToFinish signal is
             * emitted before the playback finishes. Defaults to 0.
             *
             * \return The aboutToFinish time in milliseconds.
             *
             * \see setAboutToFinishTime
             * \see aboutToFinish
             */
            qint32 aboutToFinishTime() const;

        public Q_SLOTS:
            /**
             * Set the URL the MediaObject should use as media data source.
             *
             * \param url The URL to the media data.
             */
            void setUrl(const KUrl &url);

            /**
             * Instead of a media file MediaObject can also handle other media
             * like DVD, CD and so forth. To open such a media ignore the URL
             * property and call openMedia instead. \ref url() will return an
             * empty URL then.
             *
             * \param media see \ref Media
             * \param mediaDevice A device name like '/dev/dvd' or
             * '/home/user/Videos/MyDVD'
             */
            void openMedia(Media media, const QString &mediaDevice = QString());

            /**
             * Sets the time in milliseconds the aboutToFinish signal is emitted
             * before the playback finishes. Defaults to 0.
             *
             * \param newAboutToFinishTime The new aboutToFinishTime in
             * milliseconds
             *
             * \see aboutToFinishTime
             * \see aboutToFinish
             */
            void setAboutToFinishTime(qint32 newAboutToFinishTime);

        Q_SIGNALS:
            /**
             * Emitted when the file has finished playing on its own.
             * I.e. it is not emitted if you call stop(), pause() or
             * load(), but only on end-of-file or a critical error.
             *
             * \see aboutToFinish
             */
            void finished();

            /**
             * Emitted when there are only aboutToFinishTime milliseconds left
             * for playback.
             *
             * \param msec The remaining time until the playback finishes.
             *
             * \see setAboutToFinishTime
             * \see aboutToFinishTime
             * \see finished
             */
            void aboutToFinish(qint32 msec);

            /**
             * This signal is emitted as soon as the length of the media file is
             * known or has changed. For most non-local media data the length of
             * the media can only be known after some time. At that time the
             * totalTime function can not return useful information. You have
             * to wait for this signal to know the real length.
             *
             * \param length The length of the media file in milliseconds.
             *
             * \see totalTime
             */
            void length(qint64 length);

        protected:
            MediaObject(Phonon::MediaObjectPrivate &dd, QObject *parent);

        private:
            Q_PRIVATE_SLOT(k_func(), void _k_stateChanged(Phonon::State, Phonon::State))
    };
} //namespace Phonon

// vim: sw=4 ts=4 tw=80
#endif // Phonon_MEDIAOBJECT_H
