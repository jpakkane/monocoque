project('monocoque demo', 'cpp')

sdl2 = dependency('sdl2')

embedder = find_program('embedder.py')

gen = generator(embedder,
  output : ['@BASENAME@.cpp', '@BASENAME@.h'],
  arguments : ['@INPUT@', '@OUTPUT0@', '@OUTPUT1@']
)

resources = ['blue.bmp']
gen_src = gen.process(resources)

executable('mcdemo', 'main.cpp',
  gen_src,
  dependencies : sdl2)
