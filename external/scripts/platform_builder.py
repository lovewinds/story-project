#!/usr/bin/env python3


class PlatformBuilder:
    def __init__(self,
                 config: dict=None,
                 config_platform: dict=None):
        """Specify build commands for given package in platform-dependant way.

        :param name: Specify the package name.
        :param config: Package configuration to build.
        :param config_platform: Platform-dependent package configuration
        """
        self.config = {}
        self.config_platform = {}

        if config is not None:
            for k in config.keys():
                self.config[k] = config[k]

        if config_platform is not None:
            for k in config_platform.keys():
                self.config[k] = config_platform[k]

    def log(self, fmt=None, kwargs=None):
        print(f'  {fmt}'.format(kwargs))

    def tag_log(self, fmt=None, kwargs=None):
        self.log(f'     [{self.config["name"]}]  {fmt}'.format(kwargs))

    def configure(self, build_env: dict):
        self.env = build_env

    def pre(self):
        self.log("Preprocessing ...")
        self.log("[#1] Downloading package")
        self.log("     URL: [{}]".format(self.config['url']))
        self.env.download_file(self.config['url'],
                               self.config['filename'])

        self.log("[#2] Extracting package")
        if self.config['filename'].endswith('.zip'):
            self.env.extract_zip(self.config['filename'],
                                 self.config['name'])
        else:
            self.env.extract_tarball(self.config['filename'],
                                     self.config['name'])

    def build(self):
        self.log("Building ...")

    def process(self):
        # TODO: Is separation build/process required?
        self.log("Processing ...")

    def post(self):
        self.log("Postprocessing ...")
