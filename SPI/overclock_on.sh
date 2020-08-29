#!/bin/bash

echo "Activando el modo de performance"
echo performance > /sys/devices/system/cpu/cpufreq/policy0/scaling_governor

exit 0