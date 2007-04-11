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

#ifndef PHONON_BASE_H
#define PHONON_BASE_H

#include "phonon_export.h"
#include "phonondefs.h"

class QObject;
namespace Phonon
{
    class BasePrivate;
    class BaseDestructionHandler;

    /**
     * Base class for all %Phonon frontend classes. This class is mostly an
     * implementation detail, except for the isValid() method.
     *
     * \author Matthias Kretz <kretz@kde.org>
     */
    class PHONONCORE_EXPORT Base
    {
        K_DECLARE_PRIVATE(Base)
        protected:
            /**
             * \internal
             * \param d private object
             */
            Base(BasePrivate &d);

            /**
             * \internal
             * Delete the object.
             * calls phononObjectDestroyed for all registered BaseDestructionHandler objects.
             */
            virtual ~Base();

        public:

            /**
             * Tells whether the backend provides an implementation of this
             * class.
             *
             * \return \c true if backend provides an implementation
             * \return \c false if the object is not implemented by the backend
             */
            bool isValid();

        protected:
            /**
             * \internal
             * private data pointer
             */
            BasePrivate *const k_ptr;

            /**
             * \internal
             * Returns the backend object. If the object does not exist it tries to
             * create it before returning.
             *
             * \return the Iface object, might return \c 0
             */
            QObject *iface();

        private:
            Base(const Base &);
            Base &operator=(const Base &);
    };
} //namespace Phonon

// vim: sw=4 ts=4 tw=80
#endif // PHONON_BASE_H
