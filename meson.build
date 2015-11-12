project('monocoque demo', 'cpp',
  default_options : ['c_std=gnu99'])

sdl_sub = subproject('sdl2')
sdl2_dep = sdl_sub.get_variable('sdl2_dep')
sdl2main_dep = sdl_sub.get_variable('sdl2main_dep')
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

resources = ['res/blue.bmp', 'res/red.bmp', 'res/green.bmp',
  'res/startup.wav', 'res/shoot.wav', 'res/explode.wav']
gen_src = gen.process(resources)

executable('mcdemo', 'main.cpp',
  gen_src,
  dependencies : [sdl2main_dep, sdl2_dep])
