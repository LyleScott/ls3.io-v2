---
title: "Supervisor on AWS Linux AMI"
description: ""
date: "2017-11-25"
tags:
  - "devops"
  - "aws"
  - "linux"
meta_kw:
  - "supervisor"
  - "supervisord"
  - "supervisorctl"
  - "AWS Linux"
  - "AMI"
  - "AWS Linux AMI"
---

`supervisor` isn't (or atleast wasn't for me) a package available in the YUM repository that AWS
Linux AMI images use. So, install it manually (cringe!) to get it working.

You can install it easily enough via Python's package manage `pip`... assuming `pip` is for Python
2.7.x. In the case of using the AWS Linux AMI, this is default. The thing is, this doesn't include
the system plumbing to make it start up on boot but we can fix that pretty easily as well.


```bash
# Switch over to a root user.
sudo su -

# Install the supervisor package.
pip install -U supervisor

# Generate a default config file.
mkdir -p /etc/supervisor/conf.d
echo_supervisord_conf > /etc/supervisor/supervisord.conf

# Add in the setting to look for supervisor app files in /etc/supervisor/conf.d
cat << EOF >> /etc/supervisor/supervisord.conf
[include]
files = /etc/supervisor/conf.d/*.conf
EOF

# Start up supervisord (ane make sure no errors)
supervisord
```

To make `supervisord` start upon system boot, create a quick init script for a RedHat based system.

To get my started, I found some examples over at
[https://github.com/Supervisor/initscripts](https://github.com/Supervisor/initscripts). They have
to be hacked slightly to adjust some path names. The one that I came up is as follows:

```bash
sudo su -

# The single quotes around the EOF here are necessary to not evaluate "$1" in the script.
cat << 'EOF' > /etc/init.d/supervisord
#!/bin/bash
#
# supervisord   Startup script for the Supervisor process control system
#
# Author:       Mike McGrath <mmcgrath@redhat.com> (based off yumupdatesd)
#               Jason Koppe <jkoppe@indeed.com> adjusted to read sysconfig,
#                   use supervisord tools to start/stop, conditionally wait
#                   for child processes to shutdown, and startup later
#               Erwan Queffelec <erwan.queffelec@gmail.com>
#                   make script LSB-compliant
#
# chkconfig:    345 83 04
# description: Supervisor is a client/server system that allows \
#   its users to monitor and control a number of processes on \
#   UNIX-like operating systems.
# processname: supervisord
# config: /etc/supervisor/supervisord.conf
# config: /etc/sysconfig/supervisord
# pidfile: /var/run/supervisord.pid
#
### BEGIN INIT INFO
# Provides: supervisord
# Required-Start: $all
# Required-Stop: $all
# Short-Description: start and stop Supervisor process control system
# Description: Supervisor is a client/server system that allows
#   its users to monitor and control a number of processes on
#   UNIX-like operating systems.
### END INIT INFO

# Source function library
. /etc/rc.d/init.d/functions

# Source system settings
if [ -f /etc/sysconfig/supervisord ]; then
    . /etc/sysconfig/supervisord
fi

# Path to the supervisorctl script, server binary,
# and short-form for messages.
supervisorctl=/usr/local/bin/supervisorctl
supervisord=${SUPERVISORD-/usr/local/bin/supervisord}
prog=supervisord
pidfile=${PIDFILE-/tmp/supervisord.pid}
lockfile=${LOCKFILE-/var/lock/subsys/supervisord}
STOP_TIMEOUT=${STOP_TIMEOUT-60}
OPTIONS="${OPTIONS--c /etc/supervisor/supervisord.conf}"
RETVAL=0

start() {
    echo -n $"Starting $prog: "
    daemon --pidfile=${pidfile} $supervisord $OPTIONS
    RETVAL=$?
    echo
    if [ $RETVAL -eq 0 ]; then
        touch ${lockfile}
        $supervisorctl $OPTIONS status
    fi
    return $RETVAL
}

stop() {
    echo -n $"Stopping $prog: "
    killproc -p ${pidfile} -d ${STOP_TIMEOUT} $supervisord
    RETVAL=$?
    echo
    [ $RETVAL -eq 0 ] && rm -rf ${lockfile} ${pidfile}
}

reload() {
    echo -n $"Reloading $prog: "
    LSB=1 killproc -p $pidfile $supervisord -HUP
    RETVAL=$?
    echo
    if [ $RETVAL -eq 7 ]; then
        failure $"$prog reload"
    else
        $supervisorctl $OPTIONS status
    fi
}

restart() {
    stop
    start
}

case "$1" in
    start)
        start
        ;;
    stop)
        stop
        ;;
    status)
        status -p ${pidfile} $supervisord
        RETVAL=$?
        [ $RETVAL -eq 0 ] && $supervisorctl $OPTIONS status
        ;;
    restart)
        restart
        ;;
    condrestart|try-restart)
        if status -p ${pidfile} $supervisord >&/dev/null; then
          stop
          start
        fi
        ;;
    force-reload|reload)
        reload
        ;;
    *)
        echo $"Usage: $prog {start|stop|restart|condrestart|try-restart|force-reload|reload}"
        RETVAL=2
    esac

    exit $RETVAL
EOF
```

Make it executable.

```bash
sudo chmod a+x /etc/init.d/supervisord
```

Prove it works.

```bash
sudo su -

# Kill any existing supervisord processes.
kill $(pgrep supervisor) 2> /dev/null

# This should print the available commands.
sudo service supervisord

# Start up supervisord
sudo service supervisord start

# Add to services that start on boot.
sudo chkconfig --add supervisord
sudo chkconfig supervisord on
```

To verify this all works great, reboot and the `supervisord` process should be up.

```bash
sudo reboot
```

After the reboot:

```bash
# Should list the process running.
ps -ef | grep supervisord

# Pop into the supervisor shell.
sudo su -
supervisorctl pid
```

Now you can put all your `supervisord` configs in `/etc/supervisor/conf.d` and they will be loaded
next time you reboot or run `sudo service supervisord restart`.
