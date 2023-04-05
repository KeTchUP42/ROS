#ifndef KLD_H
#define KLD_H

#define SECTION( S ) __attribute__ ((section ( S )))

#define INIT_SECTION ".ctors"
#define FINI_SECTION ".dtors"

#endif // KLD_H
