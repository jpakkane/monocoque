project('monocoque demo', 'cpp')

# Fixme, use subproject once SDL2 is ported.
sdl2 = dependency('sdl2')

# Fixme: On release builds use
# -static-libstdc++ on Linux
#'-static-libstdc++ -static-libgcc' on Windows/MinGW

embedder = find_program('embedder.py')

# Rule to convert binary files to source + header.
gen = generator(embedder,
  output : ['@BASENAME@.cpp', '@BASENAME@.h'],
  arguments : ['@INPUT@', '@OUTPUT0@', '@OUTPUT1@']
)

resources = ['blue.bmp', 'red.bmp', 'green.bmp',
  'startup.wav', 'shoot.wav', 'explode.wav']
gen_src = gen.process(resources)

executable('mcdemo', 'main.cpp',
  gen_src,
  dependencies : sdl2)
