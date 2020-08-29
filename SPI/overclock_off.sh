#!/bin/bash

echo "Desactivando el modo de performance"
echo powersave > /sys/devices/system/cpu/cpufreq/policy0/scaling_governor

exit 0