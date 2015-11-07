# Monocoque sample project

This project demonstrates a simple Gui application using the Monocoque framework. The main features are the follwing:

 - output is a single statically linked executable that can be copied and run anywhere, even on Linux
 - all resources (images etc) are embedded inside the app
 - all dependencies come as source from [wrapdb](http://wrapdb.mesonbuild.com) automatically, no need to install dependencies on any platform

## Why is is called Monocoque?

A monocoque is a construct where a small outer shell contains the innards. As an example
the cockpit of a Formula one car is a monocoque. For more info see the [Wikipedia article](https://en.wikipedia.org/wiki/Monocoque).

In a similar way the framework provides an encapsulation for your app and protects it against changing systems.

## Is it a heavy duty framework like the ones we know from Java?

Not at all. Basically all monocoque is is a project with all its dependencies statically linked in. This sample uses the [Meson build system](http://mesonbuild.com) and the corresponding Wrap dependency system which has been designed to support this use case natively. The demo app uses SDL2 for the gui because it is a high quality and small portable library. You can use any gui toolkit you want as long as it can be linked statically.

## Why would I ever want to distribute an app like this?

Every now and then you need to ship a single standalone app to people. A single standalone exe is easy to send over email. It is also trivial to keep multiple different versions on the same machine with the extra knowledge that they can not accidentally read each others' data.

## Why don't the resource images have alpha channels?

Because of [this bug](https://bugzilla.libsdl.org/show_bug.cgi?id=2515).

## Contact

`jpakkane at gmail`
