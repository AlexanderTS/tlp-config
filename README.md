TLP Advanced Power Manager GTK Configurator
-----------------------

What is TLP?
-----------------------

TLP brings you the benefits of advanced power management for Linux without the need to understand every technical detail. TLP comes with a default configuration already optimized for battery life, so you may just install and forget it. Nevertheless TLP is highly customizable to fulfil your specific requirements. (http://linrunner.de/en/tlp/tlp.html)

What does TLP Configurator do?
-----------------------

Traditional way to configure TLP is to edit text configuration file (more information about TLP configuration can be found here http://linrunner.de/en/tlp/docs/tlp-configuration.html). tlp-config brings you graphical TLP configuration with hints and useful information. It uses GTK+3, so you need GTK+3 and Glib libraries  to compile it.

Installation
-----------------------

Unstable version of tlp-config is currently availiable in Ubuntu PPA only.
For other distributions it will be packed after release.

If you haven`t installed TLP yet, run:

- # apt-add-repository ppa:linrunner/tlp
- # apt-add-repository ppa:alex-tsarkov/utilites-ppa
- # apt-get update
- # apt-get install tlp tlp-config

If TLP is already installed, run:

- # apt-add-repository ppa:alex-tsarkov/utilites-ppa
- # apt-get update
- # apt-get install tlp-config

To install on other distribution, build it manually:

- $ git clone git://github.com/AlexanderTS/tlp-config.git
- $ make
- # make install

Do not forget to install TLP itself! (http://linrunner.de/en/tlp/docs/tlp-linux-advanced-power-management.html)
