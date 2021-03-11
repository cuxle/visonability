#ifndef COMMONINFO_H
#define COMMONINFO_H

#define DEBUG

#ifdef DEBUG
#define debug(arg) qDebug() <<__func__ << arg
#else
#define debug(arg)
#endif

#endif // COMMONINFO_H
