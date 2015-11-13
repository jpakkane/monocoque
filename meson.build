project('monocoque demo', 'cpp',
  default_options : ['c_std=gnu99', 'default_library=static'])

cpp = meson.get_compiler('cpp')

sdl_sub = subproject('sdl2')
sdl2_dep = sdl_sub.get_variable('sdl2_dep')
sdl2main_dep = sdl_sub.get_variable('sdl2main_dep')

embedder = find_program('embedder.py')

# Rule to convert binary files to source + header.
gen = generator(embedder,
  output : ['@BASENAME@.cpp', '@BASENAME@.h'],
  arguments : ['@INPUT@', '@OUTPUT0@', '@OUTPUT1@']
)

resources = ['res/blue.bmp', 'res/red.bmp', 'res/green.bmp',
  'res/startup.wav', 'res/shoot.wav', 'res/explode.wav']
gen_src = gen.process(resources)

link_args = []
if cpp.get_id() == 'gcc' or host_machine.system() == 'linux'
  link_args += ['-static-libstdc++', '-static-libgcc']
endif
if cpp.get_id() == 'msvc'
  link_args += ['/SUBSYSTEM:WINDOWS']
endif

executable('mcdemo', 'main.cpp',
  gen_src,
  dependencies : [sdl2main_dep, sdl2_dep],
  link_args : link_args)
