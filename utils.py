import sys
import os
import shutil
import subprocess
import os
import os.path
import zipfile
import platform
import os.path

def get_files(path):
	""" get files.
	"""

	def getlistdir(path,l):
		dirs = os.listdir(path)
		for d in dirs:
			newpath = os.path.join( path, d )
			if os.path.isdir( newpath ):
				getlistdir( newpath, l )
			else:
				l.append( newpath )
	
	ret = []
	getlistdir( path, ret )
	return ret


def copytreeWithExt(src,dst,exts):
	files = get_files(src)
	for _from in files:

		root, ext = os.path.splitext(_from)
		if not ext in exts:
			continue
		_to = dst + _from[len(src):]
		print(_from + '\t:\t' + _to)

		if not os.path.exists(os.path.dirname(_to)):
			os.makedirs(os.path.dirname(_to))
		
		shutil.copy(_from,_to)

def call( cmd ):
	""" call command line.
	"""

	print( cmd )
	p = subprocess.Popen(cmd, shell=True)
	ret = p.wait()
	print('')


def wget( address ):
	version = sys.version_info
	if version[0] == 2:
		import urllib
		urllib.urlretrieve(address, os.path.basename(address))
	elif version[0] == 3:
		import urllib.request
		urllib.request.urlretrieve(address, os.path.basename(address))

def unzip(zip_filename):
	zip_file = zipfile.ZipFile(zip_filename, "r")
	for f in zip_file.namelist():

		if f.endswith('/'):
		        os.makedirs(f)
		else:
			version = sys.version_info
			if version[0] == 2:
				unzip_file = file(f, "wb")
				unzip_file.write(zip_file.read(f))
				unzip_file.close()
			elif version[0] == 3:
				unzip_file = open(f, "wb")
				unzip_file.write(zip_file.read(f))
				unzip_file.close();
	zip_file.close()

def rm(path):
	if os.path.exists(path):
		os.remove(path)

def rmdir(path):
	if os.path.exists(path):
		print("rmdir : " + path)
		shutil.rmtree(path)
	else:
		print("rmdir : not found " + path)

def cd(path):
	os.chdir(path)

def cdToScript():
	cd(os.path.dirname(os.path.abspath(__file__)))

def mkdir(path):
	if not os.path.exists(path):
		os.mkdir(path)

def copy(src,dst):
	print("copying from {0} to {1}".format(src, dst))
	shutil.copy(src,dst)

	
def copytree(src,dst,change=False,ignoreList = None):
	if change and os.path.exists(dst):
		rmdir(dst)

	if not os.path.exists(dst):
		shutil.copytree(src,dst,ignore=ignoreList)

