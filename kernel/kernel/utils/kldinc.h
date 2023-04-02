#ifndef KLDINC_H
#define KLDINC_H

#define SECTION( S ) __attribute__ ((section ( S )))

#define INIT_SECTION ".ctors"
#define FINI_SECTION ".dtors"

#endif // KLDINC_H
