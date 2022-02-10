# Cross compilation and execution

For the moment, for the majority of users, the primary way of obtaining or
creating CHERI programs is *cross-compilation*.  This is always something of a
chore, as most software and build systems assume that compiled programs will run
on the same architecture as the compiler.  However, we have attempted to make it
as painless as possible for the exercises herein.  If you are using the CHERI
Software Release, see the [targeted instructions](./cross-ccc-release.md).
Otherwise, we suggest using the [helper script](./cross-ccc.md) packaged with
this book, which hides most of the arcana.  However, the curious or enthusiastic
user is welcome to a more [DIY](./cross-diy.md) approach.
