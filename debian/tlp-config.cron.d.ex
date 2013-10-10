#
# Regular cron jobs for the tlp-config package
#
0 4	* * *	root	[ -x /usr/bin/tlp-config_maintenance ] && /usr/bin/tlp-config_maintenance
