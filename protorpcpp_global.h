/* Copyright (C) 2009 Frederik M.J.V

 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * LGPL is available on the internet at
 * http://www.gnu.org/licenses/lgpl-2.1.html and from Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301  USA
*/

#ifndef PROTORPCPP_GLOBAL_H
#define PROTORPCPP_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PROTORPCPP_LIBRARY)
#  define PROTORPCPPSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PROTORPCPPSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PROTORPCPP_GLOBAL_H
