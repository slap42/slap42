import argparse, os, platform, shutil, subprocess

parser = argparse.ArgumentParser("simple_example")
parser.add_argument("--bumpmajor", help="Bump the release by a major version", action=argparse.BooleanOptionalAction)
parser.add_argument("--bumpminor", help="Bump the release by a minor version", action=argparse.BooleanOptionalAction)
args = parser.parse_args()

version_file = open('res/version.txt', 'r')
version = version_file.read();
version_file.close()

if args.bumpminor == True:
  v = version.split('.')
  v[1] = str(int(v[1]) + 1)
  version_file = open('res/version.txt', 'w')
  version = '.'.join(v)
  version_file.write(version)
  version_file.close()

if args.bumpmajor == True:
  v = version.split('.')
  v[0] = 'v' + str(int(v[0][1:]) + 1)
  v[1] = '0'
  version_file = open('res/version.txt', 'w')
  version = '.'.join(v)
  version_file.write(version)
  version_file.close()

if platform.system() == 'Windows':
  print('Compiling Slap42 ' + version + ' for Windows')
  subprocess.run('cmake -G "Visual Studio 17 2022" -B release')
  subprocess.run('cmake --build release --config Release')
  os.mkdir('output')
  shutil.copytree('res', 'output/res')
  shutil.move('release/Release/Slap42.exe', 'output/Slap42.exe')
  shutil.make_archive('Slap42 ' + version + ' Windows', 'zip', 'output')
  shutil.rmtree('release')
  shutil.rmtree('output')

elif platform.system() == 'Darwin':
  print('Compiling Slap42 ' + version + ' for macOS')
  subprocess.run('cmake -G Xcode -B release -D CMAKE_OSX_ARCHITECTURES=x86_64', shell=True)
  subprocess.run('cmake --build release --config Release', shell=True)
  os.mkdir('output')
  shutil.copytree('res', 'output/res')
  shutil.move('release/Release/Slap42', 'output/Slap42')
  shutil.make_archive('Slap42 ' + version + ' macOS x86_64 (Intel)', 'zip', 'output')
  shutil.rmtree('release')
  shutil.rmtree('output')

  print('Compiling Slap42 ' + version + ' for macOS')
  subprocess.run('cmake -G Xcode -B release -D CMAKE_OSX_ARCHITECTURES=arm64', shell=True)
  subprocess.run('cmake --build release --config Release', shell=True)
  os.mkdir('output')
  shutil.copytree('res', 'output/res')
  shutil.move('release/Release/Slap42', 'output/Slap42')
  shutil.make_archive('Slap42 ' + version + ' macOS arm64 (Apple Silicon)', 'zip', 'output')
  shutil.rmtree('release')
  shutil.rmtree('output')

elif platform.system() == 'Linux':
  print('Compiling Slap42 ' + version + ' for Linux')
  subprocess.run('cmake -G "Unix Makefiles" -B release -D CMAKE_BUILD_TYPE=Release', shell=True)
  subprocess.run('cmake --build release', shell=True)
  os.mkdir('output')
  shutil.copytree('res', 'output/res')
  shutil.move('release/Slap42', 'output/Slap42')
  shutil.make_archive('Slap42 ' + version + ' Linux', 'gztar', 'output')
  shutil.rmtree('release')
  shutil.rmtree('output')

else:
  print('Could not create release - unsupported host operating system: ' + platform.system())
