# Monocoque sample project

This project demonstrates a simple Gui application using the Monocoque framework. The main features are the follwing:

 - output is a single statically linked executable that can be copied and run anywhere, even on Linux
 - all resources (images etc) are embedded inside the app
 - all dependencies come as source from [wrapdb](http://wrapdb.mesonbuild.com) automatically, no need to install dependencies on any platform

## Why is is called Monocoque?

A monocoque is a construct where a small outer shell contains the innards. As an example
the cockpit of a Formula one car is a monocoque. For more info see the [Wikipedia article](https://en.wikipedia.org/wiki/Monocoque).

In a similar way the framework provides an encapsulation for your app and protects it against changing systems.

## Why don't the resource images have alpha channels?

Because of [this bug](https://bugzilla.libsdl.org/show_bug.cgi?id=2515).

## Contact

`jpakkane at gmail`
