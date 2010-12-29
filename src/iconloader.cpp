/* -*- coding: utf-8 -*-
*   Copyright (c) 2010, Tim Kleinschmit.  This file is
*   licensed under the General Public License version 3 or later.
*   See the COPYRIGHT file.
*/

#include "iconloader.h"

#include <QtCore/QFile>
#include <QtCore/QtDebug>

QList<int> IconLoader::sizes_;

void IconLoader::Init() {
  sizes_.clear ();
  sizes_ << 22 << 32 << 48;
}

QIcon IconLoader::Load( const QString &name ) {
  QIcon ret;

  if ( name.isEmpty ())
    return ret;

#if QT_VERSION >= 0x040600
  // Try to load it from the theme initially
  ret = QIcon::fromTheme ( name );
  if ( !ret.isNull ())
    return ret;
#endif

  // Otherwise use our fallback theme
  const QString path ( ":/icons/%1x%2/%3.png" );
  foreach ( int size, sizes_ ) {
    QString filename ( path. arg( size ). arg( size ).arg ( name ));

    if ( QFile::exists ( filename ))
      ret.addFile ( filename, QSize( size, size ));
  }

  if ( ret.isNull ())
    qWarning () << "Couldn't load icon" << name;
  return ret;
}

