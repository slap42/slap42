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
  version_file.write('.'.join(v))
  version_file.close()

if args.bumpmajor == True:
  v = version.split('.')
  v[0] = 'v' + str(int(v[0][1:]) + 1)
  v[1] = '0'
  version_file = open('res/version.txt', 'w')
  version_file.write('.'.join(v))
  version_file.close()

if platform.system() == 'Windows':
  print('Compiling Slap42 ' + version + ' for Windows')
  subprocess.run('cmake -G "Visual Studio 17 2022" -B release')
  subprocess.run('cmake --build release --config Release')
  os.mkdir('output')
  shutil.copytree('res', 'output/res')
  shutil.move('release\Release\Slap42.exe', 'output\Slap42.exe')
  shutil.make_archive('Slap42', 'zip', 'output')
  shutil.rmtree('release')
  shutil.rmtree('output')
