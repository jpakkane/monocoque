# Monocoque sample project

This project demonstrates a simple Gui application using the Monocoque framework. The main features are the following:

 - output is a single statically linked executable that can be copied and run anywhere, even on Linux
 - all resources (images etc) are embedded inside the app
 - all dependencies come as source from [wrapdb](http://wrapdb.mesonbuild.com) automatically, no need to install dependencies on any platform

## Lots of frameworks do the same, what sets this one apart?

The entire build definition of the project is 33 lines long. This covers all compilation steps on Linux, OSX and Windows, as well as all supporting pieces such as downloading and using dependencies, converting resource files into source and so on.

This simplicity is made possible by the use of the [Meson build system](http://mesonbuild.com) and the corresponding Wrap dependency system which has been designed to support this use case natively.

It should be noted that Meson does not force static linking, it works just fine with dynamic linking. This sample uses static linking for simplicity.

## Is it a heavy duty framework like the ones we know from Java?

Not at all. Basically all monocoque is is a project with all its dependencies statically linked in. The demo app uses SDL2 for the gui because it is a high quality and small portable library.

## Why would I ever want to distribute an app like this?

Every now and then you need to ship a single standalone app to people. A single standalone exe is easy to send over email. It is also trivial to keep multiple different versions on the same machine while knowing that they can not accidentally read each others' data files.

## Why don't the resource images have alpha channels?

Because of [this bug](https://bugzilla.libsdl.org/show_bug.cgi?id=2515).

## Why is is called Monocoque?

A monocoque is a construct where a small outer shell contains the innards. As an example
the cockpit of a Formula one car is a monocoque. For more info see the [Wikipedia article](https://en.wikipedia.org/wiki/Monocoque).

In a similar way the framework provides an encapsulation for your app and protects it against changing systems.

## Contact

`jpakkane at gmail`
