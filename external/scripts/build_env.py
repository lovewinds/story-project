#!/usr/bin/python
import os
import sys
import time
import subprocess
import tarfile
import requests
import shutil
import multiprocessing
import errno

def enum(*sequential, **named):
	try:
		enums = dict(zip(sequential, range(len(sequential))), **named)
		reverse = dict((value, key) for key, value in enums.iteritems())
		enums['reverse_mapping'] = reverse
	except AttributeError:
		enums = dict(zip(sequential, range(len(sequential))), **named)
		reverse = dict((value, key) for key, value in enums.items())
		enums['reverse_mapping'] = reverse
	return type('Enum', (), enums)

Platform = enum('Windows', 'Linux', 'macOS', 'iOS', 'NotSupport')

class BuildEnv:
	def __init__(self, platform, build_type, working_path, verbose=False):
		self.verbose = verbose
		self.working_path = working_path
		self.platform = platform
		self.patch_path = '{}/patches'.format(self.working_path)
		self.temp_path = '{}/temp'.format(self.working_path)
		self.source_path = '{}/lib_source/{}'.format(self.working_path,
			Platform.reverse_mapping[self.platform])
		self.output_path = '{}/lib_build/{}'.format(self.working_path,
			Platform.reverse_mapping[self.platform])

		self.output_bin_path = '{}/bin'.format(self.output_path)
		self.output_lib_path = '{}/lib'.format(self.output_path)
		self.output_include_path = '{}/include'.format(self.output_path)
		self.output_packaging_path = '{}/packaging'.format(self.output_path)

		self.NJOBS = str(multiprocessing.cpu_count() / 2)
		if not self.NJOBS:
			self.NJOBS="1"

		self.BUILD_FLAG = ''
		self.BUILD_CONF = 'Release'
		if build_type == 'debug':
			self.BUILD_FLAG = 'CFLAGS="-ggdb3 -O0" CXXFLAGS="-ggdb3 -O0" '
			self.BUILD_CONF = 'Debug'

	@staticmethod
	def mkdir_p(path):
		try:
			os.makedirs(path)
		except OSError as exc: # Python >2.5
			if exc.errno == errno.EEXIST and os.path.isdir(path):
				pass
			else: raise

	def download_file(self, url, file_name, headers=None):
		# Proxy : http://bsnippet.tistory.com/45
		self.mkdir_p(self.temp_path)
		if not os.path.isfile("{}/{}".format(self.temp_path, file_name)):
			# urllib.request.urlretrieve(url,
			# 	"{}/{}".format(self.temp_path, file_name))
			req = requests.get(url, stream=True, headers=headers)
			if req.status_code == 200:
				with open("{}/{}".format(self.temp_path, file_name), 'wb') as f:
					req.raw.decode_content = True
					shutil.copyfileobj(req.raw, f)
			else:
				print("  status : {}".format(req.status_code))

			if not os.path.isfile("{}/{}".format(self.temp_path, file_name)):
				print("  Downloaded file not found! Please check internet connection, etc")
				raise FileNotFoundError

	def extract_tarball(self, archive_file, package_name):
		if os.path.exists('{}/{}'.format(self.source_path, package_name)):
			print("    [{}] already extracted.".format(package_name))
		else:
			tarfile.open('{}/{}'.format(self.temp_path, archive_file)).extractall(self.source_path)
			# Remove '.tar.gz'
			archive_name = archive_file.replace('.tar.gz', '').replace('.tgz', '')
			os.rename('{}/{}'.format(self.source_path, archive_name),
					  '{}/{}'.format(self.source_path, package_name))
			print("    [{}] extracted.".format(package_name))

	def run_command(self, cmd, module_name):
		str_out = ''
		if self.verbose:
			proc = subprocess.Popen(cmd, shell=True)
			while proc.poll() is None:
				time.sleep(1)
			(str_out, str_err) = proc.communicate()
		else:
			proc = subprocess.Popen(cmd, shell=True,
									stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
			idx = 0
			while proc.poll() is None:
				idx += 1
				time.sleep(1)
				s = "." * (idx % 10)
				sys.stdout.write("\r    [{}]                         ".format(module_name))
				sys.stdout.write("\r    [{}] {}".format(module_name, s))
				sys.stdout.flush()

			sys.stdout.write("\r    [{}]                         ".format(module_name))
			sys.stdout.write("\r    [{}] Finished !\n".format(module_name))
			(str_out, str_err) = proc.communicate()

		# Store as a log file
		now = time.localtime()
		ct = '%04d%02d%02d_%02d%02d%02d' % (
				now.tm_year, now.tm_mon, now.tm_mday,
				now.tm_hour, now.tm_min, now.tm_sec)
		log_dir = '{}/logs/{}'.format(self.working_path, module_name)
		self.mkdir_p(log_dir)
		log_file = '{}/{}.{}.log'.format(log_dir, module_name, ct)
		with open(log_file, 'w') as f:
			f.write(str_out)

		if proc.returncode != 0:
			# [TEST] Error handling
			print("")
			print("="*80)
			print("[TEST] An error has been occured !")
			print("       Check following log file:")
			print("         {}".format(log_file))
			print("="*80)
			print(str_out)

		return log_file
