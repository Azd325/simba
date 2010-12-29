/* -*- coding: utf-8 -*-
*   Copyright (c) 2010, Tim Kleinschmit.  This file is
*   licensed under the General Public License version 3 or later.
*   See the COPYRIGHT file.
*/

#ifndef ICONLOADER_H
#define ICONLOADER_H

#include <QtGui/QIcon>
#include <QtCore/QFile>
#include <QtCore/QtDebug>

class IconLoader {
public:
  static void Init();
  static QIcon Load( const QString& name );

private:
  IconLoader() {}

  static QList<int> sizes_;
};

#endif // ICONLOADER_H

